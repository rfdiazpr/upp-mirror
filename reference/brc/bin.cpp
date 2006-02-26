#include <Core/Core.h>
#include "bin.brc"
#include <plugin/bz2/bz2.h>

//**

CONSOLE_APP_MAIN
{
	puts(String().Cat() << "brc length: " << bin_brc_length);
	fflush(stdout);
	puts(String().Cat() << "cpp length: " << bin_cpp_length);
	fflush(stdout);
	puts(String().Cat() << "all count:  " << bin_all_count);
	fflush(stdout);
//	puts(String().Cat() << "cpp: <<<\n" << (const char *)bin_cpp << "\n>>>");
	puts(String().Cat() << "cpp: <<<\n" << BZ2Decompress(MemReadStream(bin_cpp, bin_cpp_length)) << "\n>>>");
	fflush(stdout);
//	puts(String().Cat() << "brc: <<<\n" << (const char *)bin_brc << "\n>>>");
	puts(String().Cat() << "brc: <<<\n" << BZ2Decompress(MemReadStream(bin_brc, bin_brc_length)) << "\n>>>");
	fflush(stdout);
	for(int i = 0; i < bin_all_count; i++) {
		puts(String().Cat() << "all length[" << i << "]: " << bin_all_length[i]);
		fflush(stdout);
		puts(String().Cat() << "all files[" << i << "]: " << bin_all_files[i]);
		fflush(stdout);
		puts(String().Cat() << "all[" << i << "]: <<<\n" << BZ2Decompress(MemReadStream(bin_all[i], bin_all_length[i])) << "\n>>>");
		fflush(stdout);
	}
}
