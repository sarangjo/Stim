struct simd_word {
    __m128i val;

    inline static simd_word tile8(uint8_t pattern) {
        return {_mm_set1_epi8(pattern)};
    }

    inline static simd_word tile16(uint16_t pattern) {
        return {_mm_set1_epi16(pattern)};
    }

    inline static simd_word tile32(uint32_t pattern) {
        return {_mm_set1_epi32(pattern)};
    }

    inline static simd_word tile64(uint64_t pattern) {
        return {_mm_set1_epi64x(pattern)};
    }

    inline operator bool() const { // NOLINT(hicpp-explicit-conversions)
        auto p = (uint64_t *)&val;
        return p[0] | p[1];
    }

    inline simd_word &operator^=(const simd_word &other) {
        val = _mm_xor_si128(val, other.val);
        return *this;
    }

    inline simd_word &operator&=(const simd_word &other) {
        val = _mm_and_si128(val, other.val);
        return *this;
    }

    inline simd_word &operator|=(const simd_word &other) {
        val = _mm_or_si128(val, other.val);
        return *this;
    }

    inline simd_word operator^(const simd_word &other) const {
        return {_mm_xor_si128(val, other.val)};
    }

    inline simd_word operator&(const simd_word &other) const {
        return {_mm_and_si128(val, other.val)};
    }

    inline simd_word operator|(const simd_word &other) const {
        return {_mm_or_si128(val, other.val)};
    }

    inline simd_word andnot(const simd_word &other) const {
        return {_mm_andnot_si128(val, other.val)};
    }

    inline simd_word leftshift_tile64(uint8_t offset) {
        return {_mm_slli_epi64(val, offset)};
    }

    inline simd_word rightshift_tile64(uint8_t offset) {
        return {_mm_srli_epi64(val, offset)};
    }

    inline uint16_t popcount() const {
        auto p = (uint64_t *)&val;
        return popcnt64(p[0]) + popcnt64(p[1]);
    }

    /// For each 128 bit word pair between the two registers, the byte order goes from this:
    /// [a0 a1 a2 a3 ... a14 a15] [b0 b1 b2 b3 ... b14 b15]
    /// to this:
    /// [a0 b0 a1 b1 ...  a7  b7] [a8 b8 a9 b9 ... a15 b15]
    inline void do_interleave8_tile128(simd_word &other) {
        auto t = _mm_unpackhi_epi8(val, other.val);
        val = _mm_unpacklo_epi8(val, other.val);
        other.val = t;
    }
};