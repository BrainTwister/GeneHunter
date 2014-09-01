#ifndef FILEIO_H_
#define FILEIO_H_

#include "GeneAssemblerException.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/nvp.hpp>
#include <fstream>
#include <string>

namespace GeneAssembler {

template < class T >
void readXML( T& data, const std::string& name, const boost::filesystem::path& filename )
{
    if (!exists(filename)) throw GeneAssemblerException("readXML: " + filename.string() + " not exist");
    std::ifstream in(filename.string().c_str());
    boost::archive::xml_iarchive ia(in);
    ia >> boost::serialization::make_nvp(name.c_str(),data);
}

template < class T >
void writeXML( T& data, const std::string& name, const boost::filesystem::path& filename,
    bool overwrite = true )
{
    if (!overwrite and exists(filename)) throw GeneAssemblerException("writeXML: " + filename.string() + " exist");
    std::ofstream out(filename.string().c_str());
    boost::archive::xml_oarchive oa(out);
    oa << boost::serialization::make_nvp(name.c_str(),data);
}

template < class T >
void readBinary( T& data, const std::string& name, const boost::filesystem::path& filename )
{
	if (!exists(filename)) throw GeneAssemblerException("readBinary: " + filename.string() + " not exist");
	try {
		std::ifstream in(filename.string().c_str());
		boost::archive::binary_iarchive ia(in);
		ia >> boost::serialization::make_nvp(name.c_str(),data);
	} catch ( ... ) {
		throw GeneAssemblerException("readBinary: serialization error.");
	}
}

template < class T >
void writeBinary( T& data, const std::string& name, const boost::filesystem::path& filename,
    bool overwrite = true )
{
    if (!overwrite and exists(filename)) throw GeneAssemblerException("writeBinary: " + filename.string() + " exist");
    std::ofstream out(filename.string().c_str());
    boost::archive::binary_oarchive oa(out);
    oa << boost::serialization::make_nvp(name.c_str(),data);
}

template < class T >
void readExtensionDependend( T& data, const std::string& name, const boost::filesystem::path& filename )
{
    if (filename.string().substr(filename.string().find_last_of('.')+1) == "bin") readBinary(data,name,filename);
    else if (filename.string().substr(filename.string().find_last_of('.')+1) == "xml") readXML(data,name,filename);
    else throw GeneAssemblerException("readExtensionDependend: unkown extenion " + filename.string());
}

template < class T >
void writeExtensionDependend( T& data, const std::string& name, const boost::filesystem::path& filename,
    bool overwrite = true )
{
    if (filename.string().substr(filename.string().find_last_of('.')+1) == "bin") writeBinary(data,name,filename,overwrite);
    else if (filename.string().substr(filename.string().find_last_of('.')+1) == "xml") writeXML(data,name,filename,overwrite);
    else throw GeneAssemblerException("writeExtensionDependend: unkown extenion " + filename.string());
}

} // namespace GeneAssembler

#endif /* FILEIO_H_ */
