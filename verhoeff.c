#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BASE   58

int mult[BASE][BASE];
int permutation[12][BASE];
unsigned char alphabet[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

void init(int base)
{
	assert(base > 2);
	assert(base % 2 == 0);
	/* Build multiplication table as N-based dihedral group */
	int i, j;
	for (i=0; i<base; i++)
	{
		mult[0][i] = i;
		mult[1][i] = i+1;
		mult[base/2][i] = base-i;
	}
	mult[1][base/2-1] = mult[base/2][base/2] = 0;
	mult[1][base-1] = mult[base/2][0] = base/2;
	for (i=2; i<base; i++)
	{
		if (i == base/2)
			continue;
		for (j=0; j<base; j++)
			mult[i][j] = mult[1][mult[i-1][j]];
	}
	/* Build permutation */
	for (i=0; i<base; i++)
		permutation[0][i] = i;
	permutation[1][0] = 0;
	for (i=1; i<base/2; i++)
		permutation[1][i] = base-i;
	for (i=base/2; i<base; i++)
		permutation[1][i] = i-base/2+1;
	/* Pre-build premutation table */
	for (i=2; i<12; i++)
		for (j=0; j<base; j++)
			permutation[i][j] = permutation[i-1][permutation[1][j]];
}

int check_char(int *str, size_t len, int start)
{
	int p = 0;
	int i;
	for (i=len-1; i>=0; i--)
		p = mult[p][permutation[(len-i-1+start)%12][str[i]]];
	return p >= BASE/2 ? p :
		p ? BASE/2-p : 0;
}

int is_valid(int *str, size_t len)
{
	return check_char(str, len, 0) == 0;
}

int main(int argc, char *argv[])
{
	int map[1 << (sizeof(char)*8)];
	int *str;
	int i;
	unsigned char *p;

	if (argc < 2)
	{
		puts("Usage: verhoeff <string>");
		return 0;
	}
	init(BASE);
	/* build map from chars to numbers */
	for (i=0; i<sizeof(map)/sizeof(map[0]); i++)
		map[i] = -1;
	for (i=0; i<BASE; i++)
		map[(int)alphabet[i]] = i;
	/* map string to array of ints */
	str = malloc(sizeof(int) * (strlen(argv[1]) + 1));
	i = 0;
	for (p=(unsigned char *)argv[1]; *p; p++)
	{
		int c = map[(int)*p];
		if (c == -1)
		{
			fprintf(stderr, "Incorrect character [%c]\n", *p);
			return 1;
		}
		str[i++] = c;
	}
	/* get checksum by Verhoeff algorithm */
	str[i] = check_char(str, i, 1);
	printf("%s%c\n", argv[1], alphabet[str[i]]);
	if (!is_valid(str, i+1))
	{
		fprintf(stderr, "Internal error\n");
		return 1;
	}
	return 0;
}
