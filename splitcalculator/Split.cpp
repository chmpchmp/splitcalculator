#include "Split.h"

Split::Split() {

}

void Split::update(const string & split) {
	main_split = split;
}

void Split::save() {
	subsplits.push_back(main_split);
}