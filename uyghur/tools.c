// tools

#ifndef H_TOOLS
#define H_TOOLS

void tools_assert(bool value, const char *msg)
{
    //
}

void tools_error(const char *msg)
{
    printf("[%s] => %s ...\n", LANG_ERR, msg);
}

void tools_check(bool value, const char *msg)
{
    if (value)
        return;
    tools_error(msg);
    exit(1);
}

const char *tools_format(const char *temp, const char *msg)
{
    char txt[1024];
    sprintf(txt, temp, msg);
    const char *t = txt;
    return t;
}

char *tools_read_file(const char *path)
{
    char *text;
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    long lSize = ftell(file);
    text = (char *)malloc(lSize + 1);
    rewind(file);
    fread(text, sizeof(char), lSize, file);
    text[lSize] = '\0';
    fclose(file);
    return text;
}

char *tools_str_new(char *str, int extraLen)
{
    size_t len = strlen(str);
    char *dest = malloc(len + 1 + extraLen);
    dest[len + extraLen] = '\0';
    return dest;
}

char *tools_str_apent(char *str, char c, bool notFree)
{
    size_t len = strlen(str);
     char *dest = tools_str_new(str, 1);
    strcpy(dest, str);
    dest[len] = c;
    if (!notFree)
    {
        free(str);
    }
    return dest;
}

#endif
