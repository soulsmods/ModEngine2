#pragma once

namespace dantelion {

    struct BND4Header {
        char magic[4];
        uint8_t unk_04;
        uint8_t unk_05;
        uint8_t unk_06;
        uint8_t unk_07;

        uint8_t unk_08;
        uint8_t big_endian;
        uint8_t bit_big_endian;
        uint8_t unk_0B;

        uint32_t file_count;
        uint64_t header_size;
        char version[8];
        uint64_t file_headers_size;
        uint64_t file_headers_end;

        uint8_t unicode;
        uint8_t format;
        uint8_t extended;
        uint8_t unk_33;
        uint32_t unk_34;
        uint64_t buckets_offset;
    };

    // BND file struct has many variants depending on the bits set in the header format field
    struct BND4File_UncompressedSize_ID_NameOffset {
        uint8_t flags;
        uint8_t unk_01;
        uint8_t unk_02;
        uint8_t unk_03;

        uint32_t unk_04;
        uint64_t compressed_size;
        uint64_t uncompressed_size;
        uint32_t data_offset;
        uint32_t id;
        uint32_t name_ofset;
    };
}