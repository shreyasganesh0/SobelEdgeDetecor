int build_huffman_table(hft_t *hft) {

    int code = 0;
    int curr_pos = 0;
    for (int i = 1; i <= 16; i++) {

        hashmap_t hashmap = hashmap_create(2000);
       
        int curr_count = hft->code_count_arr[i - 1];
        u8 rep = 1;
        rep = rep << curr_pos;
        while (curr_count) {
            char symbol = *hft->symbol_bytes;
            
            hashmap_insert(symbol, rep);
            rep++;
            curr_count--;
        }
        curr_pos++;



   hft->code_size_arr; 
