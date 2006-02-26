#pragma BLITZ_APPROVE

#ifndef TOPIC_VERA
#define TOPIC_VERA 0
#define TOPIC_VERB 0
#else
	#if TOPIC_VERA == 0
		#undef TOPIC_VERA
		#define TOPIC_VERA 1
	#elif TOPIC_VERA == 1
		#undef TOPIC_VERA
		#define TOPIC_VERA 2
	#elif TOPIC_VERA == 2
		#undef TOPIC_VERA
		#define TOPIC_VERA 3
	#elif TOPIC_VERA == 3
		#undef TOPIC_VERA
		#define TOPIC_VERA 4
	#elif TOPIC_VERA == 4
		#undef TOPIC_VERA
		#define TOPIC_VERA 5
	#elif TOPIC_VERA == 5
		#undef TOPIC_VERA
		#define TOPIC_VERA 6
	#elif TOPIC_VERA == 6
		#undef TOPIC_VERA
		#define TOPIC_VERA 7
	#elif TOPIC_VERA == 6
		#undef TOPIC_VERA
		#define TOPIC_VERA 7
	#elif TOPIC_VERA == 6
		#undef TOPIC_VERA
		#define TOPIC_VERA 7
	#elif TOPIC_VERA == 7
		#undef TOPIC_VERA
		#define TOPIC_VERA 8
	#elif TOPIC_VERA == 8
		#undef TOPIC_VERA
		#define TOPIC_VERA 9
	#elif TOPIC_VERA == 9
		#undef TOPIC_VERA
		#define TOPIC_VERA 0
		#if TOPIC_VERB == 0
			#undef TOPIC_VERB
			#define TOPIC_VERB 1
		#elif TOPIC_VERB == 1
			#undef TOPIC_VERB
			#define TOPIC_VERB 2
		#elif TOPIC_VERB == 2
			#undef TOPIC_VERB
			#define TOPIC_VERB 3
		#elif TOPIC_VERB == 3
			#undef TOPIC_VERB
			#define TOPIC_VERB 4
		#elif TOPIC_VERB == 4
			#undef TOPIC_VERB
			#define TOPIC_VERB 5
		#elif TOPIC_VERB == 5
			#undef TOPIC_VERB
			#define TOPIC_VERB 6
		#elif TOPIC_VERB == 6
			#undef TOPIC_VERB
			#define TOPIC_VERB 7
		#elif TOPIC_VERB == 6
			#undef TOPIC_VERB
			#define TOPIC_VERB 7
		#elif TOPIC_VERB == 6
			#undef TOPIC_VERB
			#define TOPIC_VERB 7
		#elif TOPIC_VERB == 7
			#undef TOPIC_VERB
			#define TOPIC_VERB 8
		#elif TOPIC_VERB == 8
			#undef TOPIC_VERB
			#define TOPIC_VERB 9
		#elif TOPIC_VERB == 9
			#undef TOPIC_VERB
			#define TOPIC_VERB 10
		#elif TOPIC_VERB == 10
			#undef TOPIC_VERB
			#define TOPIC_VERB 11
		#elif TOPIC_VERB == 11
			#undef TOPIC_VERB
			#define TOPIC_VERB 12
		#elif TOPIC_VERB == 12
			#undef TOPIC_VERB
			#define TOPIC_VERB 13
		#elif TOPIC_VERB == 13
			#undef TOPIC_VERB
			#define TOPIC_VERB 14
		#elif TOPIC_VERB == 14
			#undef TOPIC_VERB
			#define TOPIC_VERB 15
		#elif TOPIC_VERB == 15
			#undef TOPIC_VERB
			#define TOPIC_VERB 16
		#elif TOPIC_VERB == 16
			#undef TOPIC_VERB
			#define TOPIC_VERB 17
		#elif TOPIC_VERB == 17
			#undef TOPIC_VERB
			#define TOPIC_VERB 18
		#elif TOPIC_VERB == 18
			#undef TOPIC_VERB
			#define TOPIC_VERB 19
		#elif TOPIC_VERB == 19
			#undef TOPIC_VERB
			#define TOPIC_VERB 20
		#else
			#error Too many language files !
		#endif
	#endif
#endif



#define GROUP(package, group) \
        { VectorMap<String, VectorMap<int, Topic> >& base = TopicBase().GetAdd(package).GetAdd(group);

#define TOPIC(id)       { Topic& topic = group.GetAdd(id);
#define REF(x)
#define STYLESHEET(x)
#define TITLE(x)        topic.title = x;// DUMP(topic.title);
#define TOPIC_TEXT(x)   topic.text << x;// DUMP(strlen(x)); DUMP(topic.text.GetLength());
#define END_TOPIC       topic.text.Shrink(); topic.title.Shrink(); }

VectorMap<String, Topic>& TopicGroup__(const char *topicfile);

INITBLOCK_(COMBINE3(TOPIC__, TOPIC_VERB, TOPIC_VERA))
{
	VectorMap<String, Topic>& group = TopicGroup__(ASSTRING(TOPICFILE));
	
	#include TOPICFILE
}

#undef TOPIC
#undef REF
#undef STYLESHEET
#undef TITLE
#undef TOPIC_TEXT
#undef END_TOPIC

#undef END_GROUP

#undef TOPICFILE
