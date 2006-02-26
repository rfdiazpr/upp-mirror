#include "Core.h"

VectorMap<String, VectorMap<String, VectorMap<String, Topic > > >& TopicBase()
{
	static VectorMap<String, VectorMap<String, VectorMap<String, Topic> > > tb;
	return tb;
}

String TopicLinkString(const TopicLink& tl)
{
	return String().Cat() << "topic://" << tl.package << '/' << tl.group << '/' << tl.topic;
}

TopicLink ParseTopicLink(const char *link)
{
	TopicLink tl;
	if(memcmp(link, "topic://", 8) == 0)
		link += 8;
	do {
		if(!IsNull(tl.package))
			tl.package << '/';
		tl.package.Cat(tl.group);
		tl.group = tl.topic;
		const char *b = link;
		while(*link && *link != '/')
			link++;
		tl.topic = String(b, link);
	}
	while(*link++);
	return tl;
}

static StaticCriticalSection sTopicLock;

Topic GetTopic(const String& package, const String& group, const String& topic)
{
	CriticalSection::Lock __(sTopicLock);
	VectorMap<String, VectorMap<String, Topic> > *p = TopicBase().FindPtr(package);
	if(p) {
		VectorMap<String, Topic> *g = p->FindPtr(group);
		if(g)
			return g->Get(topic, Topic());
	}
	return Topic();
}

Topic GetTopic(const char *path)
{
	TopicLink tl = ParseTopicLink(path);
	return GetTopic(tl.package, tl.group, tl.topic);
}

VectorMap<String, Topic>& TopicGroup__(const char *topicfile)
{
	CriticalSection::Lock __(sTopicLock);
	ASSERT(*topicfile == '<');
	ASSERT(*GetFileName(topicfile).Last() == '>');
	String q = GetFileFolder(topicfile + 1);
	String group = GetFileTitle(q);
	String package = UnixPath(GetFileFolder(q));
	return TopicBase().GetAdd(package).GetAdd(group);
}
