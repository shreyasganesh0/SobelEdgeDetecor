dqt_t dqt_arr[4];
hft_t hft_arr[4];

int main(int argc, char *argv[]) {

    FILE *fs = fopen("image2025.jpg", "w+"); 
    
    char marker[2];
    if (fread(marker, 2, fs) < 2 || *marker&0xD8 != 0xD8) {
        printf("Not a jpg\n");
    }

    
    char buf[4096];
    while ((num_bytes = fread(buf, 4096, fs)) > 0) {
        
        u8 *marker = buf;
        u8 *ptr = buf;
        marker++;
        ptr++;
        if ((*marker&0xD0 < 8) || (*marker&0xE0 == 0xE0) || (*marker&0xC0 = 0xC0)) {
           //irelevant 
            continue;
        }

        u16 *field_length;

        switch (*marker) {

            case 0xDB: //dqt case
            {
                ptr++;
                field_length = ptr;

                ptr += 2;
                u8 idx = *ptr&0x0F;
                dqt_t *dqt = &dqt_arr[idx];

                dqt->idx = idx; 
                dqt->data_size = *field_length - 3;
                if ((*ptr >> 4)) {
                    dqt->prec = 16BIT_PREC;
                    dqt->data_16 = malloc(dqt->data_size);
                    dt->data_8 = NULL; 

                    ptr++;
                    memcpy(dqt->data_16, ptr, dqt->data_size);
                } else {
                    dqt->prec = 8BIT_PREC;
                    dqt->data_8 = malloc(dqt->data_size);
                    dt->data_16 = NULL; 

                    ptr++;
                    memcpy(dqt->data_8, ptr, dqt->data_size);
                }
            }

            case 0xC4:
            {
                ptr++;
                field_length = ptr;

                ptr += 2;
                u8 idx = *ptr&0x0F;
                hft_t *hft = &hft_arr[idx];

                hft->idx = idx; //maybe not needed?
                hft->data_size = *field_length - 3;//exculding length and header bytes
                
                ptr++;
                memcpy(htf->symbol_bytes, ptr, 16);
                ptr += 16;
                htf->symbol_bytes = malloc(data->data_size - 16);
                memcpy(htf->symbol_bytes, ptr, data->data_size - 16);
                build_huffman_table(hft);
            }

            







                

                










        





            

            


