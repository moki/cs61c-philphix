#include "lib/hash_table/hash-table.h"
#include "lib/char_vector/char-vector.h"
#include "lib/djb2/djb2.h"
#include "philphix.h"

int main(int argc, char **argv){
	if(argc != 2){
		fprintf(stderr, "Specify a dictionary\n");
		return 0;
	}

	uint32_t kvsc, i;

	hash_table_t *dictionary = hash_table_constructor(1 << 3, djb2, (key_len_fn_t) strlen);

	char **ss = readDictionary(argv[1], &kvsc);

	for (i = 0; i < kvsc - 1; i += 2)
		hash_table_insert(dictionary, ss[i], ss[i + 1]);

	processInput(dictionary);

	for (i = 0; i < kvsc; i++)
		free(ss[i]);

	free(ss);

	hash_table_destructor(dictionary);

	return 0;
}

char **readDictionary(char *name, uint32_t *kvsc){
	int file = open(name, O_RDONLY);
	char buffer;
	int i, j, k;

	if (file == -1)
		return NULL;

	ssize_t n_read;

	char_vector_t *cs = char_vector_constructor();
	char *s = NULL;
	char **ss = NULL;

	i = j = k = 0;

	while ((n_read = read(file, &buffer, BUFFER_SIZE)) > 0) {
		if (!isspace(buffer)) {
			char_vector_add(cs, buffer);

			i++;
		} else {
			s = malloc(sizeof(char) * i + 1);
			if (!s) exit(EXIT_FAILURE);


			memcpy(s, char_vector_get_raw_array(cs) + k, i);

			s[i] = '\0';

			ss = realloc(ss, sizeof(char *) * (j + 1));
			if (!ss) exit(EXIT_FAILURE);

			*(ss + j++) = s;

			k += i;
			i = 0;
		}
	}

	char_vector_destructor(cs);
	close(file);

	*kvsc = j;

	return ss;
}

void processInput(hash_table_t *dictionary) {
	char_vector_t *cs = char_vector_constructor();
	char          *s, *_s;
	ssize_t        n_read;
	char 	       buffer;
	uint32_t       i, j, k;

	_s = s = NULL;
	i = k = 0;

	while ((n_read = read(STDIN_FILENO, &buffer, BUFFER_SIZE) > 0)) {
		if (isalnum(buffer)) {
			char_vector_add(cs, buffer);

			i++;
		} else {
			if (!i) {
				write(STDOUT_FILENO, &buffer, 1);
				continue;
			}

			s = realloc(s, sizeof(char) * i + 1);
			if (!s) exit(EXIT_FAILURE);

			memcpy(s, char_vector_get_raw_array(cs) + k, i);

			s[i] = '\0';

			void *value = hash_table_find(dictionary, s);

			if (value) {
				write(STDOUT_FILENO, value, strlen(value));
			} else {
				_s = realloc(_s, sizeof(char) * i + 1);
				if (!_s) exit(EXIT_FAILURE);

				memcpy(_s, s, i);

				_s[i] = '\0';

				for (j = 1; j < i; j++)
					s[j] = (char) tolower(s[j]);

				value = hash_table_find(dictionary, s);

				if (value) {
					write(STDOUT_FILENO, value, strlen(value));
				} else {
					for (j = 0; j < i; j++)
					s[j] = (char) tolower(s[j]);

					value = hash_table_find(dictionary, s);

					if (value)
						write(STDOUT_FILENO, value, strlen(value));
					else
						write(STDOUT_FILENO, _s, i);
				}

				free(_s);
				_s = NULL;
			}

			write(STDOUT_FILENO, &buffer, 1);

			k += i;
			i = 0;

			free(s);
			s = NULL;
		}
	}
}
