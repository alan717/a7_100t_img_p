void LastOcc(int(&last_occ)[5][128], std::string &vec_query) {
	size_t size = vec_query.size();
	for (size_t i = 0; i < size; ++i) {
		int length = static_cast<int>(vec_query.at(i).length());
		for (int j = length - 2; j >= 0; --j) {     // ignore the occurence of the last character of the query
			char c = vec_query.at(i).at(j);
			unsigned int ascii = static_cast<unsigned int>(c);

			if (ascii >= 65 && ascii <= 90) {       // handle case insensitive
				if (last_occ[i][ascii] == -1 && last_occ[i][ascii + 32] == -1) {
					last_occ[i][ascii] = j;
					last_occ[i][ascii + 32] = j;
				}
			}
			else if (ascii >= 97 && ascii <= 122) {
				if (last_occ[i][ascii] == -1 && last_occ[i][ascii - 32] == -1) {
					last_occ[i][ascii] = j;
					last_occ[i][ascii - 32] = j;
				}
			}
			else if (ascii == 32 && last_occ[i][ascii] == -1) {     // the occ of space
				last_occ[i][ascii] = j;
			}
		}
	}
}
int main(int argc, const char * argv[]) {
 


 
	int last_occ[5][128] = { { 0 } };         // at most 5 queries, for simplification, we declare each dimension with size of 128
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 128; ++j) {
			last_occ[i][j] = -1;        // each position has initial value -1
		}
	}

	LastOcc(last_occ, vec_query);      // initialise last_occ matrix

	std::vector<Filename_Frequency> vec_filename_frequency;     // store the file, frequency pairs
	for (size_t i = 0; i < vec_filenames.size(); ++i) {
		std::string path_filename = static_cast<std::string>(dir_path) + '/' + vec_filenames.at(i);
		Filename_Frequency f;
		f.filename = vec_filenames.at(i);
		f.frequency = 0;

		std::ifstream input_file;
		std::vector<int> vec_frequency_tmp;
		for (size_t j = 0; j < vec_query.size(); ++j) {
			input_file.open(path_filename.c_str());
			int frequency = BoyerMooreHorspool(vec_query.at(j), last_occ, j, input_file);





#ifdef TEST
	std::ifstream ifile;
	std::string path_filename = static_cast<std::string>(dir_path) + '/' + vec_filenames.at(0);
	ifile.open(path_filename.c_str());

			

#else



#endif
	return 0;
}