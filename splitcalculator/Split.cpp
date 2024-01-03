#include "Split.h"

void Split::increment(const string & split) {
	main_split = split;
}

void Split::update(const string & finished_level) {
	subsplits.push_back(main_split);
}