#include <utility>
#include <string>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_binary.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/stl.hpp>

#include "packets.hpp"

namespace tftp_common {

namespace parsers {

using boost::spirit::qi::phrase_parse;
using boost::spirit::qi::_1;
using boost::spirit::byte_;
using boost::spirit::word;
using boost::phoenix::push_back;
using boost::phoenix::ref;

template <typename Iterator>
bool parse_prq_wrq_packet(Iterator first, Iterator last, tftp_common::packets::prq& packet) {
	std::string Filename;
	std::string Mode;

	bool r = phrase_parse(
		first, last,
		(
			(word(01) | word(02)) // PRQ/WRQ opcodes (2 bytes)
			>> (byte_[push_back(boost::phoenix::ref(Filename), _1)] >> byte_(0)) // Filename (string)
			>> (byte_[push_back(boost::phoenix::ref(Mode), _1)] >> byte_(0)) // Mode (string)
		)
	);

	if (!r || first != last)
		return false;
	packet = tftp_common::packets::prq { std::move(Filename), std::move(Mode) };
	return r;
}

template <typename Iterator>
bool parse_data_packet(Iterator first, Iterator last, tftp_common::packets::data& packet) {
	uint16_t block;
	std::vector<std::byte> data;

	bool r = phrase_parse(
		first, last,
		(
			(word(03)) // Data opcode (2 bytes)
			>> (word[ref(block) = _1]) // Block # (2 bytes)
			>> (byte_[push_back(boost::phoenix::ref(data), _1)])
		)
	);

	if (!r || first != last)
		return false;
	packet = tftp_common::packets::data { block, std::move(data) };
	return r;
}

template <typename Iterator>
bool parse_ack_packet(Iterator first, Iterator last, tftp_common::packets::ack& packet) {
	uint16_t block;

	bool r = phrase_parse(
		first, last,
		(
			(word(04)) // Ack opcode (2 bytes)
			>> (word[ref(block) = _1]) // Block # (2 bytes)
		)
	);

	if (!r || first != last)
		return false;
	packet = tftp_common::packets::ack { block };
	return r;
}

template <typename Iterator>
bool parse_error_packet(Iterator first, Iterator last, tftp_common::packets::error& packet) {
	uint16_t ErrorCode;
	std::string ErrorMsg;

	bool r = phrase_parse(
		first, last,
		(
			(word(05)) // Error opcode (2 bytes)
			>> (word[ref(ErrorCode) = _1]) // ErrorCode (2 bytes)
			>> (byte_[push_back(boost::phoenix::ref(ErrorMsg), _1)] >> byte_(0)) // ErrMsg (string)
		)
	);

	if (!r || first != last)
		return false;
	packet = tftp_common::packets::error { ErrorCode, std::move(ErrorMsg) };
	return r;
}

}

}
