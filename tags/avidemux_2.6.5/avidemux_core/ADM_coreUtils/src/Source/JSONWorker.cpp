#include "JSONWorker.h"

#ifdef JSON_READ_PRIORITY

JSONNode JSONWorker::parse(const json_string & json) json_throws(std::invalid_argument) {
    json_auto<json_char> s;
    #if defined JSON_DEBUG || defined JSON_SAFE
	   json_char lastchar;
	   s.set(RemoveWhiteSpace(json, lastchar));
	   return _parse_unformatted(s.ptr, lastchar);
    #else
	   s.set(RemoveWhiteSpace(json));
	   return _parse_unformatted(s.ptr);
    #endif
}

JSONNode JSONWorker::parse_unformatted(const json_string & json) json_throws(std::invalid_argument) {
    #if defined JSON_DEBUG || defined JSON_SAFE
	   #ifndef JSON_NO_EXCEPTIONS
		  JSON_ASSERT_SAFE((json[0] == '{') || (json[0] == '['), JSON_TEXT("Not JSON!"), throw std::invalid_argument(EMPTY_STD_STRING););
	   #else
		  JSON_ASSERT_SAFE((json[0] == '{') || (json[0] == '['), JSON_TEXT("Not JSON!"), return JSONNode(JSON_NULL););
	   #endif
	   json_char temp = (json[0] == '{') ? '}' : ']';
	   return _parse_unformatted(json.c_str(), temp);
    #else
	   return _parse_unformatted(json.c_str());
    #endif
}

#if defined JSON_DEBUG || defined JSON_SAFE
JSONNode JSONWorker::_parse_unformatted(const json_char * json, json_char & lastchar) json_throws(std::invalid_argument) {
#else
JSONNode JSONWorker::_parse_unformatted(const json_char * json) json_throws(std::invalid_argument) {
#endif
    #ifdef JSON_COMMENTS
	   json_char firstchar = *json;
	   json_string _comment;
	   json_char * runner = (json_char*)json;
	   if (json_unlikely(firstchar == '\5')){  //multiple comments will be consolidated into one
		  newcomment:
		  while(*(++runner) != '\5'){
			 JSON_ASSERT(*runner, JSON_TEXT("Removing white space failed"));
			 _comment += *runner;
		  }
		  firstchar = *(++runner); //step past the trailing tag
		  if (json_unlikely(firstchar == '\5')){
			 _comment += '\n';
			 goto newcomment;
		  }
	   }
    #else
	   const json_char firstchar = *json;
    #endif

    switch (firstchar){
        case '{':
        case '[':
		  #if defined JSON_DEBUG || defined JSON_SAFE
			 if (firstchar == '['){
				if (json_unlikely(lastchar != ']')){
				    JSON_FAIL(JSON_TEXT("Missing final ]"));
				    break;
				}
			 } else {
				if (json_unlikely(lastchar != '}')){
				    JSON_FAIL(JSON_TEXT("Missing final }"));
				    break;
				}
			 }
		  #endif
		  #ifdef JSON_COMMENTS
			 JSONNode foo(runner);
			 foo.set_comment(_comment);
			 return JSONNode(true, foo);  //forces it to simply return the original interal, even with ref counting off
		  #else
			 return JSONNode(json);
		  #endif
    }

    JSON_FAIL(JSON_TEXT("Not JSON!"));
    #ifndef JSON_NO_EXCEPTIONS
	   throw std::invalid_argument(EMPTY_STD_STRING);
    #else
	   return JSONNode(JSON_NULL);
    #endif
}
#endif

#define QUOTECASE()\
    case JSON_TEXT('\"'):\
	   while (*(++p) != JSON_TEXT('\"')){\
		  JSON_ASSERT_SAFE(*p, JSON_TEXT("Null terminator inside of a quotation"), return json_string::npos;);\
	   }\
	   break;

#if defined(JSON_DEBUG) || defined(JSON_SAFE)
    #define NULLCASE(error)\
	   case JSON_TEXT('\0'):\
		  JSON_FAIL_SAFE(error, return json_string::npos;);\
		  break;
#else
    #define NULLCASE(error)
#endif

#define BRACKET(left, right)\
    case left: {\
	   size_t brac = 1;\
	   while (brac){\
		  switch (*(++p)){\
			 case right:\
				--brac;\
				break;\
			 case left:\
				++brac;\
				break;\
			 QUOTECASE()\
			 NULLCASE(JSON_TEXT("Null terminator inside of a bracket"))\
		  }\
	   }\
	   break;}\
    case right:\
	   return json_string::npos;



#ifdef JSON_READ_PRIORITY
    size_t JSONWorker::FindNextRelevant(json_char ch, const json_string & value_t, const size_t pos) json_nothrow {
	   const json_char * start = value_t.c_str();
	   for (const json_char * p = start + pos; *p; ++p){
		  if (json_unlikely(*p == ch)) return p - start;
		  switch (*p){
				BRACKET(JSON_TEXT('['), JSON_TEXT(']'))
				BRACKET(JSON_TEXT('{'), JSON_TEXT('}'))
				QUOTECASE()
		  }
	   };
	   return json_string::npos;
    }
#endif

#ifdef JSON_COMMENTS
    #define COMMENT_DELIMITER() *runner++ = '\5'
    #define AND_RUNNER ,runner
    inline void SingleLineComment(const json_char * & p, json_char * & runner) json_nothrow {
	   //It is okay to add two '\5' characters here because at minimun the # and '\n' are replaced, so it's at most the same size
	   COMMENT_DELIMITER();
	   while((*(++p)) && (*p != JSON_TEXT('\n'))){
		  *runner++ = *p;
	   }
	   COMMENT_DELIMITER();
    }
#else
    #define COMMENT_DELIMITER() (void)0
    #define AND_RUNNER
#endif

#ifndef JSON_STRICT
inline void SingleLineComment(const json_char * & p) json_nothrow {
    while((*(++p)) && (*p != JSON_TEXT('\n')));
}
#endif

#ifdef JSON_READ_PRIORITY
#if defined JSON_DEBUG || defined JSON_SAFE
    json_char * JSONWorker::RemoveWhiteSpace(const json_string & value_t, json_char & last) json_nothrow  {
#else
    json_char * JSONWorker::RemoveWhiteSpace(const json_string & value_t) json_nothrow {
#endif
	   json_char * result;
	   json_char * runner = result = json_malloc<json_char>(value_t.length() + 1);  //dealing with raw memory is faster than adding to a json_string
	   JSON_ASSERT(result != 0, JSON_TEXT("Out of memory"));
	   for(const json_char * p = value_t.c_str(); *p; ++p){
		  switch(*p){
			 case JSON_TEXT(' '):   //defined as white space
			 case JSON_TEXT('\t'):  //defined as white space
			 case JSON_TEXT('\n'):  //defined as white space
			 case JSON_TEXT('\r'):  //defined as white space
				break;
			 #ifndef JSON_STRICT
				case JSON_TEXT('/'):  //a C comment
				    if (*(++p) == JSON_TEXT('*')){  //a multiline comment
					   COMMENT_DELIMITER();
					   while ((*(++p) != JSON_TEXT('*')) || (*(p + 1) != JSON_TEXT('/'))){
						  JSON_ASSERT_SAFE(*p, JSON_TEXT("Null terminator inside of a multiline quote"), COMMENT_DELIMITER(); goto endofloop;);
						  *runner++ = *p;
					   }
					   ++p;
					   COMMENT_DELIMITER();
					   break;
				    }
				    //Should be a single line C comment, so let it fall through to use the bash comment stripper
				    JSON_ASSERT_SAFE(*p == JSON_TEXT('/'), JSON_TEXT("stray / character, not quoted, or a comment"), goto endofloop;);
				case JSON_TEXT('#'):  //a bash comment
				    SingleLineComment(p AND_RUNNER);
				    break;
			 #endif
			 case JSON_TEXT('\"'):  //a quote
				*runner++ = JSON_TEXT('\"');
				while(*(++p) != JSON_TEXT('\"')){  //find the end of the quotation, as white space is preserved within it
				    JSON_ASSERT_SAFE(*p, JSON_TEXT("Null terminator inside of a quotation"), goto endofloop;);
				    switch(*p){
					   case JSON_TEXT('\\'):
						  *runner++ = JSON_TEXT('\\');
						  *runner++ = (*++p == JSON_TEXT('\"')) ? JSON_TEXT('\1') : *p;  //an escaped quote will reak havoc will all of my searching functions, so change it into an illegal character in JSON for convertion later on
						  break;
					   default:
						  *runner++ = *p;
						  break;
				    }
				}
				//no break, let it fall through so that the trailing quote gets added
			 default:
				JSON_ASSERT_SAFE((json_uchar)*p >= 32, JSON_TEXT("Invalid JSON character detected (lo)"), goto endofloop;);
				JSON_ASSERT_SAFE((json_uchar)*p <= 126, JSON_TEXT("Invalid JSON character detected (hi)"), goto endofloop;);
				*runner++ = *p;
				break;
		  }
	   }
	   #ifdef JSON_SAFE
		  endofloop:
	   #endif
	   #if defined JSON_DEBUG || defined JSON_SAFE
		  last = *(runner - 1);
	   #endif
	   *runner = JSON_TEXT('\0');
	   return result;
    }
#endif

json_string JSONWorker::RemoveWhiteSpaceAndComments(const json_string & value_t) json_nothrow {
    json_string result;
    result.reserve(value_t.length());
    for(const json_char * p = value_t.c_str(); *p; ++p){
	   switch(*p){
		  case JSON_TEXT(' '):   //defined as white space
		  case JSON_TEXT('\t'):  //defined as white space
		  case JSON_TEXT('\n'):  //defined as white space
		  case JSON_TEXT('\r'):  //defined as white space
			 break;
		  #ifndef JSON_STRICT
			 case JSON_TEXT('/'):  //a C comment
				if (*(++p) == JSON_TEXT('*')){  //a multiline comment
				    while ((*(++p) != JSON_TEXT('*')) || (*(p + 1) != JSON_TEXT('/'))){
					   JSON_ASSERT_SAFE(*p, JSON_TEXT("Null terminator inside of a multiline quote"), goto endofloop;);
				    }
				    ++p;
				    break;
				}
				//Should be a single line C comment, so let it fall through to use the bash comment stripper
				JSON_ASSERT_SAFE(*p == JSON_TEXT('/'), JSON_TEXT("stray / character, not quoted, or a comment"), goto endofloop;);
			 case JSON_TEXT('#'):  //a bash comment
				SingleLineComment(p);
				break;
		  #endif
		  case JSON_TEXT('\"'):  //a quote
			 result += JSON_TEXT('\"');
			 while(*(++p) != JSON_TEXT('\"')){  //find the end of the quotation, as white space is preserved within it
				JSON_ASSERT_SAFE(*p, JSON_TEXT("Null terminator inside of a quotation"), goto endofloop;);
				switch(*p){
				    case JSON_TEXT('\\'):
					   result += JSON_TEXT('\\');
					   result += (*++p == JSON_TEXT('\"')) ? JSON_TEXT('\1') : *p;  //an escaped quote will reak havoc will all of my searching functions, so change it into an illegal character in JSON for convertion later on
					   break;
				    default:
					   result += *p;
					   break;
				}
			 }
			 //no break, let it fall through so that the trailing quote gets added
		  default:
			 JSON_ASSERT_SAFE((json_uchar)*p >= 32, JSON_TEXT("Invalid JSON character detected (lo)"), goto endofloop;);
			 JSON_ASSERT_SAFE((json_uchar)*p <= 126, JSON_TEXT("Invalid JSON character detected (hi)"), goto endofloop;);
			 result += *p;
			 break;
	   }
    }
    #ifdef JSON_SAFE
	   endofloop:
    #endif
    return result;
}

#ifdef JSON_READ_PRIORITY
/*
 These three functions analyze json_string literals and convert them into std::strings
 This includes dealing with special characters and utf characters
 */
#ifdef JSON_UNICODE
    inline json_uchar SurrogatePair(const json_uchar hi, const json_uchar lo) json_pure;
    inline json_uchar SurrogatePair(const json_uchar hi, const json_uchar lo) json_nothrow {
	   JSON_ASSERT(sizeof(unsigned int) == 4, JSON_TEXT("size of unsigned int is not 32-bit"));
	   JSON_ASSERT(sizeof(json_uchar) == 4, JSON_TEXT("size of json_char is not 32-bit"));
	   return (((hi << 10) & 0x1FFC00) + 0x10000) | lo & 0x3FF;
    }

    json_string JSONWorker::UTF(const json_char * & pos) json_nothrow {
	   json_string result;
	   json_uchar first = UTF8(pos);
	   if (json_unlikely((first > 0xD800) && (first < 0xDBFF) &&
		  (*(pos + 1) == '\\') && (*(pos + 2) == 'u'))){
			 const json_char * original_pos = pos;  //if the 2nd character is not correct I need to roll back the iterator
			 pos += 2;
			 json_uchar second = UTF8(pos);
			 //surrogate pair, not two characters
			 if (json_unlikely((second > 0xDC00) && (second < 0xDFFF))){
				result += SurrogatePair(first, second);
			 } else {
				pos = original_pos;
			 }
	   } else {
		  result += first;
	   }
	   JSON_ASSERT(!result.empty(), JSON_TEXT("missing case, somehow UTF returned empty"));
	   return result;
    }
#endif

json_uchar JSONWorker::UTF8(const json_char * & pos) json_nothrow {
    #ifdef JSON_UNICODE
	   ++pos;
	   json_uchar temp = Hex(pos) << 8;
	   ++pos;
	   return temp | Hex(pos);
    #else
	   JSON_ASSERT(*(pos + 1) == JSON_TEXT('0'), JSON_TEXT("wide utf character (hihi)"));
	   JSON_ASSERT(*(pos + 2) == JSON_TEXT('0'), JSON_TEXT("wide utf character (hilo)"));
	   pos += 3;
	   return Hex(pos);
    #endif
}


json_char JSONWorker::Hex(const json_char * & pos) json_nothrow {
    /*
	takes the numeric value of the next two characters and convert them
	\u0058 becomes 0x58

	In case of \u, it's SpecialChar's responsibility to move past the first two chars
	as this method is also used for \x
	*/
    //First character
    json_uchar hi = *pos++ - 48;
    if (hi > 48){  //A-F don't immediately follow 0-9, so have to pull them down a little
	   hi -= 39;
    } else if (hi > 9){  //neither do a-f
	   hi -= 7;
    }
    //second character
    json_uchar lo = *pos - 48;
    if (lo > 48){  //A-F don't immediately follow 0-9, so have to pull them down a little
	   lo -= 39;
    } else if (lo > 9){  //neither do a-f
	   lo -= 7;
    }
    //combine them
    return (json_char)((hi << 4) | lo);
}

#ifndef JSON_STRICT
    inline json_char FromOctal(const json_char * & str) json_nothrow {
	   JSON_ASSERT(json_strlen(str) > 3, JSON_TEXT("Octal will go out of bounds"));
	   str += 2;
	   return (json_char)(((((json_uchar)(*(str - 2) - 48))) << 6) | (((json_uchar)(*(str - 1) - 48)) << 3) | ((json_uchar)(*str - 48)));
    }
#endif

void JSONWorker::SpecialChar(const json_char * & pos, json_string & res) json_nothrow {
    /*
	   Since JSON uses forward slash escaping for special characters within strings, I have to
	   convert these escaped characters into C characters
	*/
    switch(*pos){
	   case JSON_TEXT('\1'):  //quote character (altered by RemoveWhiteSpace)
		  res += JSON_TEXT('\"');
		  break;
	   case JSON_TEXT('t'):	//tab character
		  res += JSON_TEXT('\t');
		  break;
	   case JSON_TEXT('n'):	//newline character
		  res += JSON_TEXT('\n');
		  break;
	   case JSON_TEXT('r'):	//return character
		  res += JSON_TEXT('\r');
		  break;
	   case JSON_TEXT('\\'):	//backslash
		  res += JSON_TEXT('\\');
		  break;
	   case JSON_TEXT('/'):	//forward slash
		  res += JSON_TEXT('/');
		  break;
	   case JSON_TEXT('b'):	//backspace
		  res += JSON_TEXT('\b');
		  break;
	   case JSON_TEXT('f'):	//formfeed
		  res += JSON_TEXT('\f');
		  break;
	   case JSON_TEXT('v'):	//vertical tab
		  res += JSON_TEXT('\v');
		  break;
	   case JSON_TEXT('u'):	//utf character
		  #ifdef JSON_UNICODE
			 res += UTF(pos);
		  #else
			 res += UTF8(pos);
		  #endif
		  break;
	   #ifndef JSON_STRICT
		  case JSON_TEXT('x'):   //hexidecimal ascii code
			 res += Hex(++pos);
			 break;

		  #ifdef __GNUC__
			 case JSON_TEXT('0') ... JSON_TEXT('7'):
		  #else
			 //octal encoding
			 case JSON_TEXT('0'):
			 case JSON_TEXT('1'):
			 case JSON_TEXT('2'):
			 case JSON_TEXT('3'):
			 case JSON_TEXT('4'):
			 case JSON_TEXT('5'):
			 case JSON_TEXT('6'):
			 case JSON_TEXT('7'):
		  #endif
			 res += FromOctal(pos);
			 break;
		  default:
			 res += *pos;
			 break;
	   #elif defined(JSON_DEBUG)
		  default:
			 JSON_FAIL(JSON_TEXT("Unsupported escaped character"));
			 break;
	   #endif
    }
}

#ifdef JSON_LESS_MEMORY
    inline void doflag(const internalJSONNode * flag, bool which, bool x) json_nothrow {
	   if (json_likely(which)){
		  flag -> _name_encoded = x;
	   } else {
		  flag -> _string_encoded = x;
	   }
    }

    json_string JSONWorker::FixString(const json_string & value_t, const internalJSONNode * flag, bool which) json_nothrow {
    #define setflag(x) doflag(flag, which, x)
#else
    json_string JSONWorker::FixString(const json_string & value_t, bool & flag) json_nothrow {
    #define setflag(x) flag = x
#endif

    //Do things like unescaping
    setflag(false);
    json_string res;
    res.reserve(value_t.length());	 //since it goes one character at a time, want to reserve it first so that it doens't have to reallocating
    for(const json_char * p = value_t.c_str(); *p; ++p){
	   switch (*p){
		  case JSON_TEXT('\\'):
			 setflag(true);
			 SpecialChar(++p, res);
			 break;
		  default:
			 res += *p;
			 break;
	   }
    }
    return shrinkString(res);  //because this is actually setting something to be stored, shrink it it need be
}
#endif

#ifdef JSON_UNICODE
    #ifdef JSON_ESCAPE_WRITES
	   json_string JSONWorker::toSurrogatePair(json_uchar C) json_nothrow {
		  JSON_ASSERT(sizeof(unsigned int) == 4, JSON_TEXT("size of unsigned int is not 32-bit"));
		  JSON_ASSERT(sizeof(unsigned short) == 2, JSON_TEXT("size of unsigned short is not 16-bit"));
		  JSON_ASSERT(sizeof(json_uchar) == 4, JSON_TEXT("json_char is not 32-bit"));

		  //Compute the high surrogate
		  unsigned short HiSurrogate = 0xD800 | (((unsigned short)((unsigned int)((C >> 16) & 31)) - 1) << 6) | ((unsigned short)C) >> 10;

		  //compute the low surrogate
		  unsigned short LoSurrogate = (unsigned short) (0xDC00 | ((unsigned short)C) & 1023);

		  json_string res;
		  res += toUTF8(HiSurrogate);
		  res += toUTF8(LoSurrogate);
		  return res;
	   }
    #endif
#endif

#ifdef JSON_ESCAPE_WRITES
    json_string JSONWorker::toUTF8(json_uchar p) json_nothrow {
	   #ifdef JSON_UNICODE
		  if (json_unlikely(p > 0xFFFF)) return toSurrogatePair(p);
	   #endif
	   json_string res(JSON_TEXT("\\u"));
	   #ifdef JSON_UNICODE
		  START_MEM_SCOPE
			 json_uchar hihi = ((p & 0xF000) >> 12) + 48;
			 if (hihi > 57) hihi += 7; //A-F don't immediately follow 0-9, so have to further adjust those
			 json_uchar hilo = ((p & 0x0F00) >> 8) + 48;
			 if (hilo > 57) hilo += 7; //A-F don't immediately follow 0-9, so have to further adjust those
			 res += hihi;
			 res += hilo;
		  END_MEM_SCOPE
		  json_uchar hi = ((p & 0x00F0) >> 4) + 48;
	   #else
		  res += JSON_TEXT("00");
		  json_uchar hi = (p >> 4) + 48;
	   #endif
	   //convert the character to be escaped into two digits between 0 and 15
	   if (hi > 57) hi += 7; //A-F don't immediately follow 0-9, so have to further adjust those
	   json_uchar lo = (p & 0x000F) + 48;
	   if (lo > 57) lo += 7; //A-F don't immediately follow 0-9, so have to further adjust those
	   res += hi;
	   res += lo;
	   return res;
    }
#endif

json_string JSONWorker::UnfixString(const json_string & value_t, bool flag) json_nothrow {
    if (!flag) return value_t;
    //Re-escapes a json_string so that it can be written out into a JSON file
    json_string res;
    res.reserve(value_t.length());  //since it goes one character at a time, want to reserve it first so that it doens't have to reallocating
    for(const json_char * p = value_t.c_str(); *p; ++p){
	   switch(*p){
		  case JSON_TEXT('\"'):  //quote character
			 res += JSON_TEXT("\\\"");
			 break;
		  case JSON_TEXT('\\'):	//backslash
			 res += JSON_TEXT("\\\\");
			 break;
		  #ifdef JSON_ESCAPE_WRITES
			 case JSON_TEXT('\t'):	//tab character
				res += JSON_TEXT("\\t");
				break;
			 case JSON_TEXT('\n'):	//newline character
				res += JSON_TEXT("\\n");
				break;
			 case JSON_TEXT('\r'):	//return character
				res += JSON_TEXT("\\r");
				break;
			 case JSON_TEXT('/'):	//forward slash
				res += JSON_TEXT("\\/");
				break;
			 case JSON_TEXT('\b'):	//backspace
				res += JSON_TEXT("\\b");
				break;
			 case JSON_TEXT('\f'):	//formfeed
				res += JSON_TEXT("\\f");
				break;
			 default:
				if (json_unlikely(((json_uchar)(*p) < 32) || ((json_uchar)(*p) > 126))){
				    res += toUTF8((json_uchar)(*p));
				} else {
				    res += *p;
				}
				break;
		  #else
			 default:
				res += *p;
				break;
		  #endif
	   }
    }
    return res;
}

#ifdef JSON_READ_PRIORITY
//Create a childnode
#ifdef JSON_COMMENTS
    #define ARRAY_PARAM bool array  //Just to supress warnings
#else
    #define ARRAY_PARAM bool
#endif
inline void JSONWorker::NewNode(const internalJSONNode * parent, const json_string & name, const json_string & value, ARRAY_PARAM) json_nothrow {
    #ifdef JSON_COMMENTS
	   JSONNode * child;
	   START_MEM_SCOPE
		  json_string _comment;
		  START_MEM_SCOPE
			 const json_char * runner = ((array) ? value.c_str() : name.c_str());
			 if (json_unlikely(*runner == '\5')){  //multiple comments will be consolidated into one
				newcomment:
				while(*(++runner) != '\5'){
				    JSON_ASSERT(*runner, JSON_TEXT("Removing white space failed"));
				    _comment += *runner;
				}
				if (json_unlikely(*(++runner) == '\5')){ //step past the trailing tag
				    _comment += '\n';
				    goto newcomment;
				}
			 }
			 internalJSONNode * myinternal;
			 if (array){
				myinternal = internalJSONNode::newInternal(name, runner);
			 } else {
				myinternal = internalJSONNode::newInternal(++runner, value);
			 }
			 child = JSONNode::newJSONNode(myinternal);
		  END_MEM_SCOPE
		  child -> set_comment(_comment);
	   END_MEM_SCOPE
	   const_cast<internalJSONNode*>(parent) -> CHILDREN -> push_back(child);   //attach it to the parent node
    #else
	if (name.empty()){
	   	const_cast<internalJSONNode*>(parent) -> CHILDREN -> push_back(JSONNode::newJSONNode(internalJSONNode::newInternal(name.c_str(), value)));	    //attach it to the parent node
	} else {
		const_cast<internalJSONNode*>(parent) -> CHILDREN -> push_back(JSONNode::newJSONNode(internalJSONNode::newInternal(name.c_str() + 1, value)));	    //attach it to the parent node
	}
    #endif
}

//Create a subarray
void JSONWorker::DoArray(const internalJSONNode * parent, const json_string & value_t) json_nothrow {
    //This takes an array and creates nodes out of them
    JSON_ASSERT(!value_t.empty(), JSON_TEXT("DoArray is empty"));
    JSON_ASSERT_SAFE(value_t[0] == JSON_TEXT('['), JSON_TEXT("DoArray is not an array"), parent -> Nullify(); return;);
    if (json_unlikely(value_t.length() <= 2)) return;  // just a [] (blank array)

    #ifdef JSON_SAFE
	   json_string newValue;  //share this so it has a reserved buffer
    #endif
    size_t starting = 1;  //ignore the [

    //Not sure what's in the array, so we have to use commas
    for(size_t ending = FindNextRelevant(JSON_TEXT(','), value_t, 1);
	   ending != json_string::npos;
	   ending = FindNextRelevant(JSON_TEXT(','), value_t, starting)){

	   #ifdef JSON_SAFE
		  newValue.assign(value_t.begin() + starting, value_t.begin() + ending);
		  JSON_ASSERT_SAFE(FindNextRelevant(JSON_TEXT(':'), newValue, 0) == json_string::npos, JSON_TEXT("Key/Value pairs are not allowed in arrays"), parent -> Nullify(); return;);
		  NewNode(parent, EMPTY_JSON_STRING, newValue, true);
	   #else
		  NewNode(parent, EMPTY_JSON_STRING, json_string(value_t.begin() + starting, value_t.begin() + ending), true);
	   #endif
		  starting = ending + 1;
    }
    //since the last one will not find the comma, we have to add it here, but ignore the final ]

    #ifdef JSON_SAFE
	   newValue.assign(value_t.begin() + starting, value_t.end() - 1);
	   JSON_ASSERT_SAFE(FindNextRelevant(JSON_TEXT(':'), newValue, 0) == json_string::npos, JSON_TEXT("Key/Value pairs are not allowed in arrays"), parent -> Nullify(); return;);
	   NewNode(parent, EMPTY_JSON_STRING, newValue, true);
    #else
	   NewNode(parent, EMPTY_JSON_STRING, json_string(value_t.begin() + starting, value_t.end() - 1), true);
    #endif
}


//Create all child nodes
void JSONWorker::DoNode(const internalJSONNode * parent, const json_string & value_t) json_nothrow {
    //This take a node and creates its members and such
    JSON_ASSERT(!value_t.empty(), JSON_TEXT("DoNode is empty"));
    JSON_ASSERT_SAFE(value_t[0] == JSON_TEXT('{'), JSON_TEXT("DoNode is not an node"), parent -> Nullify(); return;);
    if (json_unlikely(value_t.length() <= 2)) return;  // just a {} (blank node)

    size_t name_ending = FindNextRelevant(JSON_TEXT(':'), value_t, 1);  //find where the name ends
    JSON_ASSERT_SAFE(name_ending != json_string::npos, JSON_TEXT("Missing :"), parent -> Nullify(); return;);
    json_string name(value_t.begin() + 1, value_t.begin() + name_ending - 1);	  //pull the name out
    for (size_t value_ending = FindNextRelevant(JSON_TEXT(','), value_t, name_ending),  //find the end of the value
	    name_starting = 1;  //ignore the {
	    value_ending != json_string::npos;
	    value_ending = FindNextRelevant(JSON_TEXT(','), value_t, name_ending)){

	   NewNode(parent, name, json_string(value_t.begin() + name_ending + 1, value_t.begin() + value_ending), false);
	   name_starting = value_ending + 1;
	   name_ending = FindNextRelevant(JSON_TEXT(':'), value_t, name_starting);
	   JSON_ASSERT_SAFE(name_ending != json_string::npos, JSON_TEXT("Missing :"), parent -> Nullify(); return;);
	   name.assign(value_t.begin() + name_starting, value_t.begin() + name_ending - 1);
    }
    //since the last one will not find the comma, we have to add it here
    NewNode(parent, name, json_string(value_t.begin() + name_ending + 1, value_t.end() - 1), false);
}
#endif
