#include <iostream>
#include <vector>

struct tape
{
	std::istream str;
	std::vector<int> buf;
	int shift()
	{
		if(buf.size())
		{
			int retval = buf.back();
			buf.pop_back()
			return retval;
		}
		return str.get();
	}
	void push(int i)
	{ buf.push(i); }
}

struct tokenstring
{
	std::vector<int> v;
	int comp(std::vector<int> &s, int  i )
	{
		int i=0;
		for(; i < std::min(v.size(),s.size()*2); i+=2 )
		{
			int cmp = (v[i] >= s[i] ) - ( v[i+1] <= s[i] );
			if(cmp)
				return cmp;
		}
		if( v.size() >= (s.size()+1)*2 )
		{
			int cmp = (v[i] >= i ) - ( v[i+1] <= i );	
			if(cmp)
				return cmp;
			else
				return 0;
		}
		return -1;
	}
}

struct langRule
{
	tokenstring str;
	void *(*value)(std::vector<void *>);
	std::vector<int> red;
	size_t context;
};

typedef std::vector<langRule> langContext;

void parse(tape &t, std::vector<langContext> langContexts )
{

	std::vector<int> str;
	std::vector<size_t> stack;
	int i=0, j = langContexts[cid].size() - 1, m, midx = 0;

	while( i <= j )
	{
		m = ( (j-i) >> 1 )+i;
		int cmp = langContext[stack.back()][m].comp(str, c );
		if( cmp > 0 )
			i = m+1;
		else if( cmp < 0 )
			j = m-1;
		else

	}

}

int rule[] = {NUM, NUM, 'a', 'f' };
