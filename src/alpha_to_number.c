
#include <stddef.h> // Define NULL

#include "my_put_char.h"
#include "my_put_string.h"
#include "string_length.h"

int alpha_to_number(char const* str)
{
    int length = string_length(str);
    int result = 0;
    int is_negative = 0;

    for (int i = 0; i < length; ++i)
    {
        char c = str[i];
        if (i == 0 && c == '-')
        {
            is_negative = 1;
            continue;
        }

        if (c < '0' || c > '9')
            break;

        int unit = c - '0'; // e.g.: '2' - '0' => 2
        if (is_negative)
            unit = -unit;

        // Each loop, we multiply result per 10 and add the unit
        result = result * 10;
        result = result + unit;
    }

    return result;
}

#ifdef TEST_25
int main(int argc, char* argv[])
{
    int r = alpha_to_number("");
    if (r != 0)
        my_put_string("Error 0\n");

    r = alpha_to_number("123");
    if (r != 123)
        my_put_string("Error 1\n");

    r = alpha_to_number("-123");
    if (r != -123)
        my_put_string("Error 2\n");

    r = alpha_to_number("0");
    if (r != 0)
        my_put_string("Error 3\n");

    r = alpha_to_number("2147483647");
    if (r != 2147483647)
        my_put_string("Error 4\n");

    r = alpha_to_number("-2147483648");
    if (r != -2147483648)
        my_put_string("Error 5\n");

    r = alpha_to_number("a42");
    if (r != 0)
        my_put_string("Error 6\n");

    r = alpha_to_number("78bf56");
    if (r != 78)
        my_put_string("Error 7\n");
    return 0;
}
#endif

