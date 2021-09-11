#include <vector>
#include <typeinfo>

struct expression
{
	virtual void expression *clone() = 0;	
	virtual bool bind(expression *expr) = 0;
	virtual bool unbind() = 0;
};

struct function : expression
{

	std::vector<expression *> params; //maybe we should use smart pointers? we'll see

	virtual void expression *clone()
	{
		retval = new expression(*this);
		for( auto &e : retval->param )
		{
			e = new expression(e->clone());
			*e = e->clone();
		}
		return retval;
	}

	virtual bool bind(expression *expr)
	{
		if( typeid(*expr) != typeid(*this) )
			return false;
		for( auto e : param )
			if( !e->bind() )
				return false;
		return true;
	}

	virtual bool unbind()
	{
		if( typeid(*expr) != typeid(*this) )
			return false;
		for( auto e : param )
			if( !e->unbind() )
				return false;
		return true;
	}

	virtual ~function()
	{
	}

};

struct bind : expression
{
	expression *binded;
};
