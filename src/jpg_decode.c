
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

            case 0xDB:
            {
                ptr++;
                field_length = ptr;
                dqt->data_size = field_length - 3;

                ptr += 2;
                if ((*ptr >> 4)) {
                    dqt->prec = 16BIT_PREC;
                    dqt->idx = *ptr&0x0F;
                    dqt->data_16 = malloc(dqt->data_size);
                    dt->data_8 = NULL; 

                    ptr++;
                    memcpy(dqt->data_16, ptr, dqt->data_size);
                } else {
                    dqt->prec = 8BIT_PREC;
                    dqt->idx = *ptr&0x0F;
                    dqt->data_8 = malloc(dqt->data_size);
                    dt->data_16 = NULL; 

                    ptr++;
                    memcpy(dqt->data_8, ptr, dqt->data_size);
                }
            }

            case 0x









        





            

            


