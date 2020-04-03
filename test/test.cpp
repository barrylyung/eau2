#include "../src/modified_dataframe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


void benchmarker_map_sum(ModifiedDataFrame* df, Rower* rower) {
  std::chrono::high_resolution_clock::time_point h_start, h_end;
  h_start = std::chrono::high_resolution_clock::now();
  df->map(*rower);
  printf("Rower Map of size %zu took: \n", df->schema_->length());
  h_end = std::chrono::high_resolution_clock::now();
  std::cout << "highres = " << std::chrono::duration<double>(h_end - h_start).count() << " s \n";
}

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, "<");
            tok && *tok;
            tok = strtok(NULL, "<\n"))
    {
        printf("%s\n", tok);
        if (!--num)
            return tok;
    }
    return NULL;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**) malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

ModifiedDataFrame* read_from_file(char* file_path) {
  FILE* stream = fopen("data.csv", "r");
  char line[1024];
  Schema s("IIII");
  ModifiedDataFrame* df = new ModifiedDataFrame(s);
  Row  r(df->get_schema());
  while (fgets(line, 1024, stream))
  {
      char* tmp = strdup(line);
      char** tokens = str_split(tmp, ';');
      if (tokens)
        {
            int i;
            for (i = 0; *(tokens + i); i++)
            {
                int x = atoi(*(tokens + i));
                r.set(i, x);
                free(*(tokens + i));
            }
            df->add_row(r);
            free(tokens);
        }
  }
  return df;
}

int main()
{
  ModifiedDataFrame* test = read_from_file("data.csv");
  SumRower* summer = new SumRower();
  PrintRower* printer = new PrintRower();
  benchmarker_map_sum(test, printer);
  benchmarker_map_sum(test, summer);
}
