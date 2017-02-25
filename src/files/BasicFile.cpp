// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) 2015-2016 Guillaume Fraux and contributors
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/

#include <algorithm>

#include "chemfiles/files/BasicFile.hpp"
#include "chemfiles/Error.hpp"
using namespace chemfiles;

BasicFile::BasicFile(const std::string& filename, File::Mode mode)
    : TextFile(filename, mode) {
    std::ios_base::openmode openmode;

    switch (mode) {
    case File::READ:
        openmode = std::ios_base::in;
        break;
    case File::APPEND:
        openmode = std::ios_base::out | std::ios_base::app;
        break;
    case File::WRITE:
        openmode = std::ios_base::out | std::ios_base::trunc;
        break;
    default:
        throw FileError(std::string("Got a bad file mode: ") + static_cast<char>(mode));
    }

    stream_.open(filename, openmode);
    if (!stream_.is_open()) {
        throw FileError("Could not open the file " + filename);
    }
    TextFile::rdbuf(stream_.rdbuf());
    lines_.resize(1);
    rewind();
}

const std::string& BasicFile::readline() {
    std::getline(stream_, lines_[0]);
    return lines_[0];
}

void BasicFile::rewind() {
    stream_.clear();
    stream_.seekg(0, std::ios::beg);
}

bool BasicFile::eof() {
    return stream_.eof();
}

const std::vector<std::string>& BasicFile::readlines(size_t n) {
    lines_.resize(n);
    std::string line;
    for (size_t i = 0; i < n; i++) {
        std::getline(stream_, line);
        lines_[i] = line;
    }

    if (!stream_) {
        throw FileError("Error while reading file " + filename());
    }

    return lines_;
}
