#include <iostream>
#include <string.h>
#include "chezscheme.h"
#include "racketcs.h"
#include "racket_modules.hpp"

static ptr to_bytevector(char* s)
{
	iptr len = strlen(s);
	ptr bv = Smake_bytevector(len, 0);
	memcpy(Sbytevector_data(bv), s, len);
	return bv;
}

int main()
{
	racket_boot_arguments_t ba;
	memset(&ba, 0, sizeof(ba));
	ba.boot1_path = "./petite.boot";
	ba.boot2_path = "./scheme.boot";
	ba.boot3_path = "./racket.boot";
	ba.exec_file = "embedding_racket.exe";
	racket_boot(&ba);
	declare_modules();

	racket_namespace_require(Sstring_to_symbol("racket/base"));
	ptr e = to_bytevector((char*)"(display \"God is in heaven, and everything is normal on Earth.\n\")");
	e = Scons(Sstring_to_symbol("open-input-bytes"), Scons(e, Snil));
	e = Scons(Sstring_to_symbol("read"), Scons(e, Snil));
	e = Scons(Sstring_to_symbol("eval"), Scons(e, Snil));
	e = Scons(Sstring_to_symbol("println"), Scons(e, Snil));
	racket_eval(e);

	return 0;
}
