/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** testmd5.cpp
 ** This code tests the implementation of md5.hpp against precomputed md5 hashes.
 **
 ** Author: James Holland - jholland@usgs.gov
 ** -------------------------------------------------------------------------*/
#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include <cstdint>
#include "md5.hpp"

void testCase(std::string testString, std::string expectation) {

	std::vector<std::uint8_t> test(testString.begin(), testString.end());
	std::stringstream result;
	for (std::uint8_t num : Md5::hash(test))
		result << std::setfill('0') << std::setw(2) << std::hex
				<< (unsigned int) num;

	if (result.str().compare(expectation) == 0) {
		std::cout << "Pass: \"" << testString << "\"\n"; //\t"<<result.str()<<"\n\n";
	} else {
		std::cout << "Fail: \"" << testString << "\"\n";
		std::cout << "\tResult:\t\t" << result.str() << "\n";
		std::cout << "\tExpected:\t" << expectation << "\n";
	}

}
void stringTestCase(std::string testString, std::string expectation) {

	std::stringstream result;
	for (std::uint8_t num : Md5::hash(testString))
		result << std::setfill('0') << std::setw(2) << std::hex
				<< (unsigned int) num;

	if (result.str().compare(expectation) == 0) {
		std::cout << "Pass: \"" << testString << "\"\n"; //\t"<<result.str()<<"\n\n";
	} else {
		std::cout << "Fail: \"" << testString << "\"\n";
		std::cout << "\tResult:\t\t" << result.str() << "\n";
		std::cout << "\tExpected:\t" << expectation << "\n";
	}

}

int main() {
	//RFC Test cases (From the specification)
	testCase("a", "0cc175b9c0f1b6a831c399e269772661");
	testCase("abc", "900150983cd24fb0d6963f7d28e17f72");
	testCase("message digest", "f96b697d7cb7938d525a2f31aaf161d0");
	testCase("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
	testCase("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
			"d174ab98d277d9f5a5611c2c9f419d9f");
	testCase(
			"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
			"57edf4a22be3c955ac49da2e2107b67a");

	//Other test Cases
	testCase("", "d41d8cd98f00b204e9800998ecf8427e");
	testCase("The quick brown fox jumps over the lazy dog",
			"9e107d9d372bb6826bd81d3542a419d6");
	testCase("The quick brown fox jumps over the lazy dog.",
			"e4d909c290d0fb1ca068ffaddf22cbd0");
	testCase("The quick brown fox jumps over the lazy cog",
			"1055d3e698d289f2af8663725127bd4b");
	testCase("The quick brown fox jumped over the lazy dog's back",
			"e38ca1d920c4b8b8d3946b2c72f01680");
	testCase(
			"Denna programvara bör kunna beräkna md5 hash för någon vektor byte.",
			"5785575f90b647d3832280ce92fcca50");
	testCase(
			"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque hendrerit dolor eget nunc fermentum, ac pretium risus tincidunt. Aenean porttitor semper mi, in tincidunt urna interdum id. Fusce placerat libero mauris, ac ornare orci dapibus et. Aliquam pharetra nisl fermentum ex dignissim, vitae pellentesque tortor aliquet. Ut eu leo at felis ornare luctus. Mauris sollicitudin et diam non ultricies. Sed dapibus eros nec cursus semper. Pellentesque ut malesuada turpis.Nullam vel accumsan mauris, suscipit lobortis sem. Vestibulum a justo non nulla imperdiet consequat a sit amet nisi. Etiam eu purus nec diam finibus luctus at id libero. Pellentesque eget nisi at odio lacinia sagittis vitae ac ligula. Quisque ac mauris consectetur, suscipit mi quis, aliquam massa. Pellentesque dictum felis turpis, a mollis urna accumsan in. Duis laoreet eros ut euismod ullamcorper. Integer vel dolor fringilla, vulputate enim vitae, tristique mi. Sed posuere eros non pulvinar molestie. Ut a turpis venenatis, condimentum magna vitae, feugiat justo. Etiam tincidunt vel mauris dignissim malesuada. Donec eget viverra tortor, et aliquam lorem. Integer vitae ligula vitae nunc pulvinar ornare non facilisis enim. Vestibulum vitae scelerisque augue. Vestibulum cursus auctor enim at mollis.Ut posuere commodo nunc et commodo. Nunc tincidunt sapien hendrerit neque vulputate interdum. Sed id est consequat, accumsan tortor vitae, volutpat metus. Cras sit amet ante sit amet nisl maximus semper non cursus arcu. Aliquam et semper libero. Morbi blandit scelerisque libero eget scelerisque. Integer elementum molestie erat, id viverra erat rutrum ac. Aliquam ullamcorper nulla orci, luctus commodo libero tempus et. Vivamus interdum molestie urna egestas tristique. Ut finibus lobortis odio, ac facilisis eros aliquet ut.Donec et aliquet magna, et venenatis ligula. Cras eleifend dui ut magna facilisis euismod. Curabitur et facilisis diam, dictum tempor tortor. In sem sem, placerat ac scelerisque nec, eleifend a arcu. Cras pellentesque vehicula nibh non ornare. Donec pretium massa at augue pretium imperdiet. Aliquam semper consectetur dolor, nec lobortis orci tristique a. Integer sed tincidunt lectus. Nunc mattis mattis dapibus. Aliquam vitae iaculis ipsum, id condimentum elit. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis tempus, leo sed mollis tempor, mauris ligula finibus nibh, at gravida eros felis ut magna.Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Vestibulum vitae nibh eu dui auctor convallis. Aliquam enim lorem, semper eu nulla hendrerit, pharetra luctus mi. Vivamus nisl sapien, consectetur nec ante porta, aliquet blandit nibh. Phasellus commodo ut velit ornare vestibulum. Nullam eu ante tempus nisi congue blandit vel at leo. Nulla dapibus nunc vel suscipit cursus. Integer dignissim interdum nunc, eget tincidunt odio consectetur sit amet. Cras id efficitur metus, eget facilisis libero. Aliquam scelerisque lacinia purus, non scelerisque ante. Phasellus urna eros, sollicitudin et ullamcorper eu, elementum eu augue. Curabitur venenatis metus augue, sed mollis sem scelerisque eget. Curabitur semper vestibulum egestas.",
			"f6eeef2667eae408318ad9bb49a1ee4f");
	stringTestCase(
			"Denna programvara bör kunna beräkna md5 hash för någon vektor byte.",
			"5785575f90b647d3832280ce92fcca50");
}
