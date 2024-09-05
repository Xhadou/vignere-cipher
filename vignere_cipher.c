#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LEN 10000
const double englishFreq[26] = {
    0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 
    0.002, 0.008, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 
    0.063, 0.091, 0.028, 0.010, 0.023, 0.001, 0.020, 0.001
}; // Frequency distribution of letters in the English language

// Helper Functions
double calculateIndexOfCoincidence(char *text);
char *readFile(const char *filename);
void writeFile(const char *filename, const char *content);

// Vigenere Encryption function
char* Vig_Enc(char *plaintext, char *key) {
    int i, j = 0;
    int key_len = strlen(key);
    int text_len = strlen(plaintext);
    
    char* ciphertext = malloc(text_len + 1);
    
    for (i = 0; i < text_len; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((plaintext[i] - base + toupper(key[j % key_len]) - 'A') % 26) + base;
            j++;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[i] = '\0';
    return ciphertext;
}

// Vigenere Decryption function
char* Vig_Dec(char *ciphertext, char *key) {
    int i, j = 0;
    int key_len = strlen(key);
    int text_len = strlen(ciphertext);
    
    char* plaintext = malloc(text_len + 1);
    
    for (i = 0; i < text_len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = ((ciphertext[i] - base - (toupper(key[j % key_len]) - 'A') + 26) % 26) + base;
            j++;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[i] = '\0';
    return plaintext;
}

// Vigenere Cryptanalysis function
int VigAnalysis(char *ciphertext) {
    int len = strlen(ciphertext);
    double IC, avgIC;
    char normalized[len + 1];
    int normalized_len = 0;
    int bestKeyLength = 0;
    double bestAvgIC = 0.0;

    // Normalize text to uppercase and remove non-alphabetic characters
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            normalized[normalized_len++] = toupper(ciphertext[i]);
        }
    }
    normalized[normalized_len] = '\0';

    for (int prediction = 1; prediction <= 55; prediction++) {
        avgIC = 0.0;
        int validGroups = 0;

        for (int i = 0; i < prediction; i++) {
            int groupSize = 0;
            char group[normalized_len + 1];
            int index = 0;

            for (int j = i; j < normalized_len; j += prediction) {
                group[index++] = normalized[j];
                groupSize++;
            }
            group[index] = '\0';

            if (groupSize > 1) {
                IC = calculateIndexOfCoincidence(group);
                avgIC += IC;
                validGroups++;
            }
        }

        if (validGroups > 0) {
            avgIC /= validGroups;
        }

        printf("Possible Guess for Key length: %d Avg IC: %f\n", prediction, avgIC);

        // Declare key length if Avg IC is between 0.066 and 0.071
        if (avgIC > 0.066 && avgIC < 0.071) {
            printf("Best matching key length: %d with Avg IC: %f\n", prediction, avgIC);
            bestKeyLength = prediction;
            bestAvgIC = avgIC;
            break;
        }
    }

    return bestKeyLength;
}

char *recoverVigenereKey(char *ciphertext, int keyLength) {
    int len = strlen(ciphertext);
    char normalized[len + 1];
    int normalized_len = 0;
    char *key = malloc(keyLength + 1);

    // Normalize text to uppercase and remove non-alphabetic characters
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            normalized[normalized_len++] = toupper(ciphertext[i]);
        }
    }
    normalized[normalized_len] = '\0';

    for (int i = 0; i < keyLength; i++) {
        int freq[26] = {0};
        int groupSize = 0;

        // Collect frequency of letters in each subgroup
        for (int j = i; j < normalized_len; j += keyLength) {
            freq[normalized[j] - 'A']++;
            groupSize++;
        }

        // Find the best shift that matches English frequency
        double minChiSq = 1e9; // Set to a large value initially
        int bestShift = 0;

        for (int shift = 0; shift < 26; shift++) {
            double chiSq = 0.0;

            // Calculate Chi-squared value for this shift
            for (int k = 0; k < 26; k++) {
                double expected = groupSize * englishFreq[k];
                double observed = freq[(k + shift) % 26];
                chiSq += ((observed - expected) * (observed - expected)) / expected;
            }

            // Check if this shift is the best match
            if (chiSq < minChiSq) {
                minChiSq = chiSq;
                bestShift = shift;
            }
        }

        // Set the key character based on the best shift found
        key[i] = 'A' + bestShift;
    }
    key[keyLength] = '\0';

    return key;
}

int main() {
    char choice[2], key[100], inputFile[100], outputFile[100];
    char *input, *output;

    printf("Choose operation:\n1. Encryption\n2. Decryption\n3. Cryptanalysis\nEnter your choice (1/2/3): ");
    fgets(choice, sizeof(choice), stdin);
    
    // Set default file names
    switch (choice[0]) {
        case '1':
            strcpy(inputFile, "plaintext.txt");
            strcpy(outputFile, "ciphertext.txt");
            break;
        case '2':
            strcpy(inputFile, "ciphertext.txt");
            strcpy(outputFile, "decrypted.txt");
            break;
        case '3':
            strcpy(inputFile, "ciphertext.txt");
            break;
        default:
            printf("Invalid choice.\n");
            return 1;
    }

    // Option to change file names
    char changeFile;
    printf("Default input file is %s. Do you want to change it? (y/n): ", inputFile);
    scanf(" %c", &changeFile);
    if (changeFile == 'y' || changeFile == 'Y') {
        printf("Enter the input file name: ");
        scanf("%s", inputFile);
    }

    if ((input = readFile(inputFile)) == NULL) return 1;

    switch (choice[0]) {
        case '1':
            printf("Default output file is %s. Do you want to change it? (y/n): ", outputFile);
            scanf(" %c", &changeFile);
            if (changeFile == 'y' || changeFile == 'Y') {
                printf("Enter the output file name: ");
                scanf("%s", outputFile);
            }

            printf("Enter the key: ");
            scanf("%s", key);

            output = Vig_Enc(input, key);
            writeFile(outputFile, output);
            printf("Encryption completed. Output written to %s\n", outputFile);
            free(output);
            break;
            
        case '2':
            printf("Default output file is %s. Do you want to change it? (y/n): ", outputFile);
            scanf(" %c", &changeFile);
            if (changeFile == 'y' || changeFile == 'Y') {
                printf("Enter the output file name: ");
                scanf("%s", outputFile);
            }

            printf("Enter the key: ");
            scanf("%s", key);

            output = Vig_Dec(input, key);
            writeFile(outputFile, output);
            printf("Decryption completed. Output written to %s\n", outputFile);
            free(output);
            break;
            
        case '3':
            {
                int keyLength = VigAnalysis(input);
                if (keyLength > 0) {
                    char *foundKey = recoverVigenereKey(input, keyLength);
                    printf("The estimated key is: %s\n", foundKey);
                    free(foundKey);
                } else {
                    printf("Key length could not be determined.\n");
                }
            }
            break;
    }
    
    free(input);
    return 0;
}


double calculateIndexOfCoincidence(char *text) {
    int freq[26] = {0};
    int len = 0;
    double ic = 0.0;
    
    // Calculate frequency of each letter
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            freq[toupper(text[i]) - 'A']++;
            len++;
        }
    }
    
    // Calculate Index of Coincidence
    for (int i = 0; i < 26; i++) {
        ic += (double)freq[i] * (freq[i] - 1);
    }
    
    if (len > 1) {
        ic /= (len * (len - 1));
    }
    
    return ic;
}

char *readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }
    
    char *content = malloc(MAX_LEN * sizeof(char));
    if (content == NULL) {
        printf("Memory allocation error!\n");
        fclose(file);
        return NULL;
    }

    size_t len = fread(content, sizeof(char), MAX_LEN, file);
    content[len] = '\0';

    fclose(file);
    return content;
}

void writeFile(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
}
