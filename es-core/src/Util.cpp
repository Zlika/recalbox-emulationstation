#include "Util.h"
#include "platform.h"
#include "Locale.h"

namespace fs = boost::filesystem;

std::string strToUpper(const char* from)
{
	std::string str(from);
    return boost::locale::to_upper(str);
}

std::string& strToUpper(std::string& from)
{
    from = boost::locale::to_upper(from);
    return from;
}

std::string strToUpper(const std::string& from)
{
  return boost::locale::to_upper(from);
}

::Transform4x4f& roundMatrix(::Transform4x4f& mat)
{
	mat.translation()[0] = round(mat.translation()[0]);
	mat.translation()[1] = round(mat.translation()[1]);
	return mat;
}

::Transform4x4f roundMatrix(const ::Transform4x4f& mat)
{
	::Transform4x4f ret = mat;
	roundMatrix(ret);
	return ret;
}

::Vector3f roundVector(const ::Vector3f& vec)
{
	::Vector3f ret = vec;
	ret[0] = round(ret[0]);
	ret[1] = round(ret[1]);
	ret[2] = round(ret[2]);
	return ret;
}

::Vector2f roundVector(const ::Vector2f& vec)
{
	::Vector2f ret = vec;
	ret[0] = round(ret[0]);
	ret[1] = round(ret[1]);
	return ret;
}

// embedded resources, e.g. ":/font.ttf", need to be properly handled too
std::string getCanonicalPath(const std::string& path)
{
	if(path.empty() || !boost::filesystem::exists(path))
		return path;

	return boost::filesystem::canonical(path).generic_string();
}

std::string getExpandedPath(const std::string& str)
{
	std::string path = str;

	//expand home symbol if the startpath contains ~
	if (!path.empty() && path[0] == '~')
	{
		path.erase(0, 1);
		path.insert(0, getHomePath());
	}

	return path;
}

// expands "./my/path.sfc" to "[relativeTo]/my/path.sfc"
// if allowHome is true, also expands "~/my/path.sfc" to "/home/pi/my/path.sfc"
fs::path resolvePath(const fs::path& path, const fs::path& relativeTo, bool allowHome)
{
	// nothing here
	if(path.begin() == path.end())
		return path;

	if(*path.begin() == ".")
	{
		fs::path ret = relativeTo;
		for (auto it = ++path.begin(); it != path.end(); ++it)
			ret /= *it;
		return ret;
	}

	if(allowHome && *path.begin() == "~")
	{
		fs::path ret = getHomePath();
		for (auto it = ++path.begin(); it != path.end(); ++it)
			ret /= *it;
		return ret;
	}

	return path;
}

fs::path removeCommonPathUsingStrings(const fs::path& path, const fs::path& relativeTo, bool& contains)
{
	const std::string& pathStr = path.generic_string();
	const std::string& relativeToStr = relativeTo.generic_string();
	if (pathStr.compare(0, relativeToStr.length(), relativeToStr) == 0)
	{
		contains = true;
		return pathStr.substr(relativeToStr.size() + 1);
	}
	else
	{
		contains = false;
		return path;
	}
}

// example: removeCommonPath("/home/pi/roms/nes/foo/bar.nes", "/home/pi/roms/nes/") returns "foo/bar.nes"
fs::path removeCommonPath(const fs::path& path, const fs::path& relativeTo, bool& contains)
{
	// if either of these doesn't exist, fs::canonical() is going to throw an error
	if(!fs::exists(path) || !fs::exists(relativeTo))
	{
		contains = false;
		return path;
	}

	fs::path p = fs::canonical(path);
	fs::path r = fs::canonical(relativeTo);

	if(p.root_path() != r.root_path())
	{
		contains = false;
		return p;
	}

	fs::path result;

	// find point of divergence
	auto itr_path = p.begin();
	auto itr_relative_to = r.begin();
	while(itr_path != p.end() && itr_relative_to != r.end())
		if (*itr_path == *itr_relative_to)
		{
			++itr_path;
			++itr_relative_to;
		}
		else
			break;

	if(itr_relative_to != r.end())
	{
		contains = false;
		return p;
	}

	while(itr_path != p.end())
	{
		if(*itr_path != fs::path("."))
			result = result / *itr_path;

		++itr_path;
	}

	contains = true;
	return result;
}

// usage: makeRelativePath("/path/to/my/thing.sfc", "/path/to") -> "./my/thing.sfc"
// usage: makeRelativePath("/home/pi/my/thing.sfc", "/path/to", true) -> "~/my/thing.sfc"
fs::path makeRelativePath(const fs::path& path, const fs::path& relativeTo, bool allowHome)
{
	bool contains = false;

	fs::path ret = removeCommonPath(path, relativeTo, contains);
	if(contains)
	{
		// success
		ret = "." / ret;
		return ret;
	}

	if(allowHome)
	{
		ret = removeCommonPath(path, getHomePath(), contains);
		if(contains)
		{
			// success
			ret = "~" / ret;
			return ret;
		}
	}

	// nothing could be resolved
	return path;
}

boost::posix_time::ptime string_to_ptime(const std::string& str, const std::string& fmt)
{
	std::istringstream ss(str);
	ss.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet(fmt))); //std::locale handles deleting the facet
	boost::posix_time::ptime time;
	ss >> time;

	return time;
}
