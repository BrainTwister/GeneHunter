#ifndef CREATEDATACLASS_H_
#define CREATEDATACLASS_H_

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#define UNDERSCORE(elem) BOOST_PP_CAT(elem,_)

#define MACRO_SINGLE_INITIALIZE_ARGUMENTS(r,size,i,elem)\
    UNDERSCORE(BOOST_PP_TUPLE_ELEM(3,1,elem))(BOOST_PP_TUPLE_ELEM(3,1,elem)) BOOST_PP_COMMA_IF(BOOST_PP_SUB(BOOST_PP_SUB(size,i),1))

#define PRINT_INITIALIZE_ARGUMENTS(SEQ)\
    BOOST_PP_SEQ_FOR_EACH_I(MACRO_SINGLE_INITIALIZE_ARGUMENTS,BOOST_PP_SEQ_SIZE(SEQ),SEQ)

#define MACRO_SINGLE_CONSTRUCTOR_ARGUMENT(r,size,i,elem)\
    BOOST_PP_TUPLE_ELEM(3,0,elem) BOOST_PP_TUPLE_ELEM(3,1,elem) = BOOST_PP_TUPLE_ELEM(3,2,elem) BOOST_PP_COMMA_IF(BOOST_PP_SUB(BOOST_PP_SUB(size,i),1))

#define PRINT_CONSTRUCTOR_ARGUMENTS(SEQ)\
    BOOST_PP_SEQ_FOR_EACH_I(MACRO_SINGLE_CONSTRUCTOR_ARGUMENT,BOOST_PP_SEQ_SIZE(SEQ),SEQ)

#define MACRO_SINGLE_MEMBER(r,data,elem)\
    BOOST_PP_TUPLE_ELEM(3,0,elem) UNDERSCORE(BOOST_PP_TUPLE_ELEM(3,1,elem));

#define PRINT_CLASS_MEMBERS(SEQ)\
    BOOST_PP_SEQ_FOR_EACH(MACRO_SINGLE_MEMBER,,SEQ)

#define MACRO_SINGLE_MEMBER_SERIALIZATION(r,data,elem)\
    ar & boost::serialization::make_nvp(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3,1,elem)), UNDERSCORE(BOOST_PP_TUPLE_ELEM(3,1,elem)));

#define PRINT_CLASS_MEMBERS_SERIALIZATION(SEQ)\
    BOOST_PP_SEQ_FOR_EACH(MACRO_SINGLE_MEMBER_SERIALIZATION,,SEQ)

#define CREATE_DATA_CLASS(Name,Members)\
    class Name\
    {\
    public:\
\
        Name(PRINT_CONSTRUCTOR_ARGUMENTS(Members))\
         : PRINT_INITIALIZE_ARGUMENTS(Members)\
        {}\
\
        PRINT_CLASS_MEMBERS(Members)\
\
    private:\
\
        friend class boost::serialization::access;\
\
        template<class Archive>\
        void serialize(Archive & ar, const unsigned int version)\
        {\
            PRINT_CLASS_MEMBERS_SERIALIZATION(Members)\
        }\
    };

#endif /* CREATEDATACLASS_H_ */
