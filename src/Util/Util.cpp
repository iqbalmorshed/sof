//-----------------------------------------------
// Copyright 2009 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL license
//-----------------------------------------------
//
// Util - Common data structures and functions
//
#include <iostream>
#include <math.h>
#include "Util.h"

//
// KAlign
//

// Return the outer coordinate of the alignment
int KAlignment::contigOuterCoordinate() const
{
	if(!is_reverse)
	{
		return contig_start_pos - read_start_pos;
	}
	else
	{
		return contig_start_pos + align_length + read_start_pos;
	}
}

//
// Convert the alignment into the alignment on the reverse complement
// of the target
//
void KAlignment::flipAlignment(int targetLength)
{
	int tPos = targetLength - contig_start_pos + align_length;
	contig_start_pos = tPos;
	is_reverse = !is_reverse;
}

//
// Get the distance from thto the end of the contig
// This is in the direction of the alignment
//
int KAlignment::getDistanceToEnd(int targetLen) const
{
	int outerCoordinate = contigOuterCoordinate();
	if(!is_reverse)
		return targetLen - outerCoordinate;
	else
		return outerCoordinate;
}

// Comparse by read position
int KAlignment::compareReadPos(const KAlignment& a1, const KAlignment& a2)
{
	return a1.read_start_pos < a2.read_start_pos;
}

// Output
std::istream& operator>> (std::istream& in, KAlignment& a)
{
	in >> a.contig_id >> a.contig_start_pos;
	in >> a.read_start_pos >> a.align_length;
	in >> a.read_length >> a.is_reverse;
	return in;
}

//
// AdjInfo
//

// Input
std::istream& operator>>(std::istream& in, AdjInfo& a)
{
	std::string line;
	getline(in, line);

	// return if we've hit the end
	if(line == "")
		return in;
	
	StringVec fields = split(line, ',');
	assert(fields.size() == 4);

	std::stringstream parser0(fields[0]);
	std::stringstream parser1(fields[1]);
	std::stringstream parser2(fields[2]);
	std::stringstream parser3(fields[3]);

	parser0 >> a.from;
	parser1 >> a.to;
	parser2 >> a.dir;
	parser3 >> a.comp;
	return in;
}

//
// Interval
//
std::ostream& operator<<(std::ostream& out, const Interval& r)
{
	out << r.start << " " << r.end;
	return out;
}

std::istream& operator>>(std::istream& in, Interval& r)
{
	in >> r.start >> r.end;
	return in;
}


Interval intersect(const Interval& r1, const Interval& r2)
{
	Interval result;
	result.start = std::max(r1.start, r2.start);
	result.end = std::min(r1.end, r2.end);

	// Check for non-overlap
	if(result.end <= result.start)
	{
		result.start = 0;
		result.end = 0;
	}
	return result;
}

// Complement the SeqCoord
SeqCoord SeqCoord::complement() const
{
	assert(isExtreme());

	SeqCoord out;
	out.seqlen = seqlen;

	if(isLeftExtreme())
	{
		out.interval.start = std::max(interval.start, interval.end) + 1;
		out.interval.end = out.seqlen - 1;
	}
	else
	{
		out.interval.start = 0;
		out.interval.end = std::min(interval.start, interval.end) - 1;
	}

	if(isReverse())
		out.reverse();
	return out;
}

std::string SeqCoord::getSubstring(const std::string& str) const
{
	int left;
	int size; 
	if(interval.start < interval.end)
	{
		left = interval.start;
		size = interval.end - interval.start + 1;
	}
	else
	{
		left = interval.end;
		size = interval.start - interval.end + 1;
	}
	return str.substr(left, size);
}

// Output
std::ostream& operator<<(std::ostream& out, const SeqCoord& sc)
{
	out << sc.interval << " " << sc.seqlen;
	return out;
}

// Input
std::istream& operator>>(std::istream& in, SeqCoord& sc)
{
	in >> sc.interval >> sc.seqlen;
	return in;
}

//
// Matching
//
Matching::Matching(const SeqCoord& sc1, const SeqCoord& sc2)
{
	coord[0] = sc1;
	coord[1] = sc2;
}

Matching::Matching(int s1, int e1, int l1, int s2, int e2, int l2)
{
	coord[0] = SeqCoord(s1, e1, l1);
	coord[1] = SeqCoord(s2, e2, l2);
}

Matching Matching::swapCoords() const
{
	Matching out;
	out.coord[0] = coord[1];
	out.coord[1] = coord[0];

	// Ensure that coord[0] is not reversed
	if(out.coord[0].isReverse())
	{
		out.coord[0].flip();
		out.coord[1].flip();
	}
	return out;
}

void Matching::normalizeCoords()
{
	// The first coord should never be reversed
	assert(!coord[0].isReverse());

	if(coord[1].isReverse())
		coord[1].flip();
}

// Output
std::ostream& operator<<(std::ostream& out, const Matching& m)
{
	out << m.coord[0] << " " << m.coord[1];
	return out;
}

// Input
std::istream& operator>>(std::istream& in, Matching& m)
{
	in >> m.coord[0] >> m.coord[1];
	return in;
}

//
// Overlap
//
Overlap::Overlap(std::string i1, int s1, int e1, int l1,
                 std::string i2, int s2, int e2, int l2)
{
	id[0] = i1;
	id[1] = i2;
	match = Matching(s1, e1, l1, s2, e2, l2);
}

// Output
std::ostream& operator<<(std::ostream& out, const Overlap& o)
{
	out << o.id[0] << " " << o.id[1] << " " << o.match;
	return out;
}

// Input
std::istream& operator>>(std::istream& in, Overlap& o)
{
	in >> o.id[0] >> o.id[1] >> o.match;
	return in;
}



//
// Sequence operations
//

// Reverse complement a sequence
Sequence reverseComplement(const Sequence& seq)
{
	std::string out(seq.length(), 'A');
	size_t last_pos = seq.length() - 1;
	for(int i = last_pos; i >= 0; --i)
	{
		out[last_pos - i] = complement(seq[i]);
	}
	return out;
}

// Reverse a sequence
Sequence reverse(const Sequence& seq)
{
	std::string out(seq.length(), 'A');
	size_t last_pos = seq.length() - 1;
	for(int i = last_pos; i >= 0; --i)
	{
		out[last_pos - i] = seq[i];
	}
	return out;
}

// Complement a sequence
Sequence complement(const Sequence& seq)
{
	std::string out(seq.length(), 'A');
	size_t l = seq.length();
	for(size_t i = 0; i < l; ++i)
	{
		out[i] = complement(seq[i]);
	}
	return out;
}

// Strip the leading directories and
// the last trailling suffix from a filename
std::string stripFilename(std::string filename)
{
	std::string temp(basename(filename.c_str())); // strip leading directory
	size_t suffixPos = temp.find_last_of('.');
	if(suffixPos == std::string::npos)
	{
		return temp; // no suffix
	}
	else
	{
		return temp.substr(0, suffixPos);
	}
}

// Ensure a filehandle is open
void checkFileHandle(std::ifstream& fh, std::string fn)
{
	if(!fh.is_open())
	{
		std::cerr << "Error: could not open " << fn << " for read\n";
		exit(1);
	}	
}


// Split a string into parts based on the delimiter
StringVec split(std::string in, char delimiter)
{
	StringVec out;
	size_t lastPos = 0;
	size_t pos = in.find_first_of(delimiter);

	while(pos != std::string::npos)
	{
		out.push_back(in.substr(lastPos, pos - lastPos));
		lastPos = pos + 1;
		pos = in.find_first_of(delimiter, lastPos);
	}
	out.push_back(in.substr(lastPos));
	return out;
}

// Split a key-value pair
void splitKeyValue(std::string in, std::string& key, std::string& value)
{
	StringVec parts = split(in, CAF_SEP);
	if(parts.size() != 2)
	{
		std::cerr << "Invalid key-value pair " << in << std::endl;
		assert(false);
	}

	key = parts[0];
	value = parts[1];

	assert(key.size() > 0 && value.size() > 0 && "Invalid key-value pair");
}

