/*
data_splitter
Copyright (c) 2023, Joshua Scoggins
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <boost/program_options.hpp>
void
doAction(const std::filesystem::path& basePath, std::istream& inputFile, size_t numberOfSplits) {
    // okay, at this point we need to open the four files that we are going to
    // be dumping data into!
    auto outputs = std::make_unique<std::ofstream[]>(numberOfSplits);
    auto parentPath = basePath.parent_path();
    auto theStem = basePath.stem();
    auto theExtension = basePath.extension();
    int errorCode = 0;
    for (size_t i = 0; i < numberOfSplits; ++i) {
        // construct 0,1,2,3 versions of the source file
        std::stringstream theFilename;
        auto pathCopy = basePath;
        theFilename << pathCopy.stem().string() << "_" << i << pathCopy.extension().string();
        auto finalPath = pathCopy.parent_path() / theFilename.str();
        outputs[i].open(finalPath, std::ios::binary | std::ios::trunc | std::ios::out);
        if (outputs[i].fail()) {
            std::stringstream ss;
            ss << "Could not open " << finalPath << std::endl;
            std::string str = ss.str();
            throw std::runtime_error(str);
        }
    }

    while (inputFile.good()) {
        for (size_t i = 0; i < numberOfSplits; ++i) {
            auto value = inputFile.get();
            if (inputFile.fail() || inputFile.eof()) {
                return;
            } else {
                outputs[i].put(value);
            }
        }
    }
}
void
doAction(const std::filesystem::path& inputFile, size_t numberOfSplits) {
        if (!std::filesystem::exists(inputFile)) {
            std::stringstream msg;
            msg << "The given source input file " << inputFile << " does not exist!" << std::endl;
            std::string str = msg.str();
            throw std::runtime_error(str);
        } else if (std::filesystem::is_directory(inputFile)) {
            std::stringstream msg;
            msg << "The given source input file " << inputFile << " is a directory!" << std::endl;
            std::string str = msg.str();
            throw std::runtime_error(str);
        }
        // for now, just divide it into four different parts as we go through
        // everything else should be fine!
        // now we need to make a quad split design
        std::ifstream theSourceFile(inputFile, std::ios::binary | std::ios::in);
        if (!theSourceFile.is_open()) {
            std::stringstream msg;
            msg << "Couldn't open " << inputFile << " for reading!" << std::endl;
            std::string str = msg.str();
            throw std::runtime_error(str);
        }
        doAction(inputFile, theSourceFile, numberOfSplits);
}
int 
main(int argc, char* argv[]) {
    size_t numberOfSplits = 0;
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("source", boost::program_options::value<std::filesystem::path>(), "source file to be decomposed")
        ("divide-into", boost::program_options::value<size_t>(&numberOfSplits)->default_value(4), "number of files to split the input into!")
        ;
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }
    try {
        if (vm.count("source")) {
           std::filesystem::path inputFile = vm["source"].as<std::filesystem::path>();
           doAction(inputFile, numberOfSplits);
        }  else {
           doAction("", std::cin, numberOfSplits);
        }
        return 0;
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
