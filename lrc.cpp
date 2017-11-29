/*
	GPL v II

	Example :
		$ <This_program_name> <plain.txt> <name_files>+"<00>.lrc"
		Search spec's LRC-format on http://en.wiki.org
		Using codepage plain.txt : cp1251

	Example compile:
		$ g++ -o <This_program_name> <this_code_name.cpp>

	$ g++ -v
	Using built-in specs.
	Target: i486-linux-gnu
	Configured with: ../src/configure -v --enable-languages=c,c++,fortran,objc,obj-c++,treelang
		--prefix=/usr --enable-shared --with-system-zlib --libexecdir=/usr/lib
		--without-included-gettext --enable-threads=posix --enable-nls --program-suffix=-4.1
		--enable-__cxa_atexit --enable-clocale=gnu --enable-libstdcxx-debug
		--enable-mpfr --with-tune=i686 --enable-checking=release i486-linux-gnu
	Thread model: posix
	gcc version 4.1.2 20061115 (prerelease) (Debian 4.1.1-21)

	irramail at gmail dot com 21/09/2008
*/

#include <iostream>
#include <fstream>
#include <iomanip>

void error(const char *er) {
	std::cout << "Error: " << er << std::endl;
	exit (EXIT_FAILURE);
}


class LRCofstream : public std::ofstream {
	char filename[40];
	int tm, div;

public:
	LRCofstream(const char *name = "book00.lrc",
                const int &art = 3, const int &ard = 3) : std::ofstream (name) {
		strcpy (filename, name);
		tm = art;
		div = ard;
	}
	void in2out (std::ifstream &in);
	void nameCountAdd ();
	void top ();
	void time2out ( const int &tt, const int &t ) {
		*this << "[" << std::setfill('0') << std::setw(2) << tt ;
		*this << ":" << std::setfill('0') << std::setw(2) << t << ".00]" ;
	}

	void timer (const int &div );

	void all (std::ifstream &in) {
		while (!in.eof()) {
			timer ( div );
			in2out ( in );
		}
	}
};

void LRCofstream::in2out(std::ifstream &in) {
		char ch[36+1];	//Для UTF-8 максимальное количество букв в слове: 18 * 2 байта + 1 байт '\0'
		int l=0;
		while ( l<36 && !in.eof() ) {
			in >> ch;
			l += strlen(ch) + 2;
			ch[36]='\0';
			if ( !in.eof() ) *this << ch << ' ';
		}
		if ( !in.eof() ) *this << std::endl;
}

void LRCofstream::nameCountAdd () {
	char r, rr;
	int l;
	l = strlen (filename) - 5 ;
	r = filename[ l--];			// *X?.lrc = -5 with end
	rr = filename[ l++ ]; 	//	*?X.lrc = -4 with end
	r++;
	if ( r>'9' ) { rr++; r='0'; }
	if ( rr>'9' ) error("Can't work with big IN file.");
	filename[ l-- ] = r;
	filename[ l ] = rr;
}

void LRCofstream::top () {
	*this << "[ti:" << filename << "]" 	<< std::endl;
//	*this << "[ar:Name writer]" 						<< std::endl;
	*this << "[la:ru]" 													<< std::endl;
	*this << "[Encoding:cp1251]"					<< std::endl;
}

void LRCofstream::timer ( const int &div ) {
	int t = tm % 60, tt=tm/60;
	if ( 59 < (t+=div) ) { t -= 60; tt +=1; }
	if (tt==3 && t>20) {
		close();
		nameCountAdd();
		open(filename);
		top();
		t = div;
		tt = 0;
		time2out(tt, t);
		tm = div;
		return;
	}
	time2out( tt, t);
	tm += div;
}

int main ( int argc, char *argv[]) {
	char filename[35] = "";
	if (argc<2) error ("$ <This_program_name> <plain.txt> <book> = > book+\"<00>.lrc\"");
	if (argc>2 ) strcat ( filename, argv[2] );
		else strcat ( filename, argv[1] );
  strcat( filename, "00.lrc");

  std::ifstream in ( argv[1] , std::ios::in | std::ios::binary );
	if ( !in ) error ( "Can't open IN file." );
	LRCofstream out ( filename );
	if ( !out ) error ( "Can't open OUT file." );

	out.top();
	out.all ( in );

	in.close ();
	out.close();

	return EXIT_SUCCESS;
}
