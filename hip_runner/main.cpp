#include <hip/hip_runtime.h>
#include <hip/hiprtc.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

/*
This is a runner for a hip module that uses the LLVM transformer to convert
The goal is to get the llvm-ir in bitcode format and then link it and run it

*/
template<typename T>
std::vector<T> load_file(const std::string& filename, std::ios_base::openmode mode = std::ios::binary) {
    std::vector<T> buffer;
    std::ifstream file(filename, mode);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        buffer.resize(size / sizeof(T));
        file.read(reinterpret_cast<char*>(buffer.data()), size);
    }
    return buffer;
}

void save_file(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
}

bool compile(const std::string& source_code, const std::vector<std::string>& options, std::vector<uint8_t>& output) {

    hiprtcProgram program;
    hiprtcResult result = hiprtcCreateProgram(&program, source_code.c_str(), "main_program", 0, nullptr, nullptr);
    if (result != HIPRTC_SUCCESS) {
        std::cerr << "Error creating HIPRTC program: " << hiprtcGetErrorString(result) << std::endl;
        return false;
    }

    const char** options_cstr = new const char*[options.size()];
    for (size_t i = 0; i < options.size(); ++i) {
        options_cstr[i] = options[i].c_str();
    }

    result = hiprtcCompileProgram(program, static_cast<int>(options.size()), options_cstr);
    if (result != HIPRTC_SUCCESS) {
        size_t log_size;
        result = hiprtcGetProgramLogSize(program, &log_size);
        char* log = new char[log_size+1];
        result = hiprtcGetProgramLog(program, log);
        log[log_size] = '\0';  // Null-terminate the log
        std::cerr << "Error compiling HIPRTC program: " << (log_size > 0 ? log : "No log available") << std::endl;
        delete[] log;
        result = hiprtcDestroyProgram(&program);
        delete[] options_cstr;
        return false;
    }
    delete[] options_cstr;

    size_t code_size;
    result = hiprtcGetBitcodeSize(program, &code_size);
    if (result != HIPRTC_SUCCESS) {
        std::cerr << "Error getting code size: " << hiprtcGetErrorString(result) << std::endl;
        hiprtcDestroyProgram(&program);
        return false;
    }

    output.resize(code_size);
    if (hiprtcGetBitcode(program, reinterpret_cast<char*>(output.data())) != HIPRTC_SUCCESS) {
        std::cerr << "Error getting bitcode: " << hiprtcGetErrorString(result) << std::endl;
        hiprtcDestroyProgram(&program);
        return false;
    }

    hiprtcDestroyProgram(&program);
    return true;
}


struct BitcodeImage {
    std::vector<uint8_t> data;
    std::string name;
    hiprtcJITInputType input_type;
};

bool link(const std::vector<BitcodeImage>& bitcodes, const std::vector<std::string>& options, std::vector<uint8_t>& output) {
    
    hiprtcLinkState link_state;
    hiprtcResult result = hiprtcLinkCreate(0, nullptr, nullptr, &link_state);
    if (result != HIPRTC_SUCCESS) {
        std::cerr << "Error creating link state: " << hiprtcGetErrorString(result) << std::endl;
        return false;
    }

    for (const auto& bitcode : bitcodes) {
        result = hiprtcLinkAddData(link_state, bitcode.input_type,
                                   const_cast<void*>(static_cast<const void*>(bitcode.data.data())),
                                   bitcode.data.size(),
                                   bitcode.name.c_str(),
                                   0, nullptr, nullptr);
        if (result != HIPRTC_SUCCESS) {
            std::cerr << "Error adding input to link state: " << hiprtcGetErrorString(result) << std::endl;
            hiprtcLinkDestroy(link_state);
            return false;
        }
    }

    size_t linked_code_size;
    void* linked_code;
    result = hiprtcLinkComplete(link_state, &linked_code, &linked_code_size);
    if (result != HIPRTC_SUCCESS) {
        std::cerr << "Error completing link state: " << hiprtcGetErrorString(result) << std::endl;
        hiprtcLinkDestroy(link_state);
        return false;
    }
    output.resize(linked_code_size);
    std::memcpy(output.data(), linked_code, linked_code_size);

    result = hiprtcLinkDestroy(link_state);
    return true;
}

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <path_to_hip_module.bc> <kernel_name>" << std::endl;
        return 1;
    }

    
    std::string hip_module_path = argv[1];
    std::string kernel_name = argv[2];

    std::cout << "HIP Opaque Pointers Transformer Runner" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "CWD: " << std::filesystem::current_path() << std::endl;
    std::cout << "HIP Module Path: " << hip_module_path << std::endl;
    std::cout << "Kernel Name: " << kernel_name << std::endl;

    if (!std::filesystem::is_regular_file(hip_module_path)) {
        std::cerr << "Error: " << hip_module_path << " is not a valid file." << std::endl;
        return 1;
    }

    std::vector<uint8_t> hip_module_data = load_file<uint8_t>(hip_module_path, std::ios::binary);
    if (hip_module_data.empty()) {
        std::cerr << "Error: Failed to load HIP module from " << hip_module_path << std::endl;
        return 1;
    }
    std::cout << "Loaded HIP module data of size: " << hip_module_data.size() << " bytes." << std::endl;

    
    hipError_t hip_status = hipInit(0);
    if (hip_status != hipSuccess) {
        std::cerr << "Error initializing HIP: " << hipGetErrorString(hip_status) << std::endl;
        return 1;
    }

    hipDeviceProp_t device_prop;
    hip_status = hipGetDeviceProperties(&device_prop, 0);
    if (hip_status != hipSuccess) {
        std::cerr << "Error getting device properties: " << hipGetErrorString(hip_status) << std::endl;
        return 1;
    }
    std::cout << "Device Name: " << device_prop.name << std::endl;
    std::cout << "Device arch: " << device_prop.gcnArchName << std::endl;

    auto main_program_source = load_file<char>("D:/Sandbox/OpaquePointersLLVMTransformer/main_program.cpp", std::ios::in);

    std::string device_arch_option = "--offload-arch=" + std::string(device_prop.gcnArchName);
    std::vector<uint8_t> main_module_data;
    std::vector<std::string> options = {
        "-fgpu-rdc"
    };


    if (!compile(reinterpret_cast<const char*>(main_program_source.data()), options, main_module_data)) {
        std::cerr << "Error compiling HIP kernel." << std::endl;
        return 1;
    }
    std::cout << "Compiled HIP kernel successfully. Size: " << main_module_data.size() << " bytes." << std::endl;
    save_file("main_program.bc", main_module_data);
    std::vector<BitcodeImage> bitcodes = 
    {
        {
            hip_module_data,
            "external",
            hiprtcJITInputType::HIPRTC_JIT_INPUT_LLVM_BITCODE
        },
        {
            main_module_data,
            "main_program",
            hiprtcJITInputType::HIPRTC_JIT_INPUT_LLVM_BUNDLED_BITCODE
        }
    };

    std::vector<std::string> link_options = {};
    std::vector<uint8_t> linked_module_data;
    if (!link(bitcodes, link_options,  linked_module_data)) {
        std::cerr << "Error linking HIP kernel." << std::endl;
        return 1;
    }

    hipModule_t hip_module;
    hip_status = hipModuleLoadData(&hip_module, linked_module_data.data());
    if (hip_status != hipSuccess) {
        std::cerr << "Error loading HIP module: " << hipGetErrorString(hip_status) << std::endl;
        return 1;
    }

    hipFunction_t hip_function;
    hip_status = hipModuleGetFunction(&hip_function, hip_module, kernel_name.c_str());
    if (hip_status != hipSuccess) {
        std::cerr << "Error getting HIP function: " << hipGetErrorString(hip_status) << std::endl;
        return 1;
    }

    std::cout << "End of HIP extern compilation and link"
              << "-------------------------------------" << std::endl;
    return 0;
}