
template <typename Integer, std::enable_if_t<std::is_integral<Integer>::value, bool> = true>
constexpr auto integer_to_varint(Integer input)
{
	constexpr size_t byteCount = (sizeof(Integer) * CHAR_BIT + CHAR_BIT - 2) / (CHAR_BIT - 1);

	struct {
		std::array<uint8_t, byteCount> bytes;
		size_t index;
	} result{};

	typedef std::make_unsigned<Integer>::type UInteger;
//	auto value = std::bit_cast<UInteger>(input);
	auto value = *(UInteger*)&input;

	for (size_t i = 1; i <= byteCount; ++i)
	{
		result.bytes[byteCount - i] |= value & (uint8_t(-1) >> 1);
		if (value >>= CHAR_BIT - 1)
			result.bytes[byteCount - i - 1] |= uint8_t(-1) & ~(uint8_t(-1) >> 1);
		else
		{
			result.index = byteCount - i;
				break;
		}
	}
	return result;
}
