gcc -o sobel_base src/sobel_filter.c

./sobel_base > outputs/output_base.txt

for script in scripts/*.sh; do
    bash "$script"
done

