#ifndef CARD_H
#define CARD_H
extern FILE* card_txt_file;
extern FILE* card_bin_file;

void initCARD();

void freeCARD();

void add_card ();

void file_to_card();

void card_to_file();

void seek_card_list_exact();

void seek_card_list_fuzzy();

void delete_card();

#endif