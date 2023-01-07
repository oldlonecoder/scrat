#include <scrat/object>
namespace scrat
{



	_object_name(object)


	scrat::object::object()
	{

	}

	scrat::object::object(object* parent_)
	{
		if (parent_ != nullptr)
		{
			_parent = parent_;
			_parent->add_child(this);
		}
	}

	scrat::object::~object()
	{
		//for (auto child : _children) delete child;
	}

	object::iterator object::query(object* obj)
	{
		//auto it = _children.begin();
		return  std::find(_children.begin(), _children.end(), obj);
	}

	void scrat::object::detach(object* obj)
	{
		auto child = query(obj);
		if (child != _children.end()) _children.erase(child);
	}

	result<> scrat::object::add_child(object* obj)
	{
		if (query(obj) != _children.end())
			return rem::notempty;

		_children.push_back(obj);
		return rem::ok;
	}
}
