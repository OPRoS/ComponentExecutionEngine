<!--
** Copyright (C) 2008 ETRI, All rights reserved
**  
**  
** This programs is the production of ETRI research & development activity;
** you cannot use it and cannot redistribute it and cannot modify it and
** cannnot  store it in any media(disk, photo or otherwise) without the prior
** permission of ETRI.
** 
** You should have received the license for this program to use any purpose.
** If not, contact the Electronics and Telecommunications Research Institute
** [ETRI], DaeDog DanJi, TaeJeon, 305-350, Korea.
** 
** NO PART OF THIS WORK BY THE THIS COPYRIGHT HEREON MAY BE REPRODUCED, STORED
** IN RETRIEVAL SYSTENS, IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
** PHOTOCOPYING, RECORDING OR OTHERWISE, WITHOUT THE PRIOR PERMISSION OF ETRI
**
** @Author: sby (sby@etri.re.kr)
**
** Generater revision 2.0
** feature:
** opros::archive compatiable;
** enabled symbolic service
-->
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method = "text"/>
<xsl:param name="filename"/>
<xsl:param name="usage"/>
<xsl:template name="componentHeader"> <!-- match="component" -->
<xsl:param name="filename"/>
#ifndef _<xsl:value-of select="name"/>_COMPONENT_H
#define _<xsl:value-of select="name"/>_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include &lt;Component.h&gt;
#include &lt;InputDataPort.h&gt;
#include &lt;OutputDataPort.h&gt;
#include &lt;InputEventPort.h&gt;
#include &lt;OutputEventPort.h&gt;
#include &lt;Event.h&gt;
#include &lt;OPRoSTypes.h&gt;

<xsl:if test="ports/*/data_type">
<xsl:for-each select="ports/*">
	<xsl:variable name="data_type" select="data_type"/>
	<xsl:variable name="data_name" select="name"/>
	<xsl:choose>
	<xsl:when test="count(//ports/*[data_type=$data_type])=1">
		<xsl:choose>
		<xsl:when test="starts-with($data_type,'std::')">
		<xsl:choose>
		<xsl:when test="contains($data_type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after($data_type,'std::'),'&lt;'))"/>&gt;
		</xsl:when>
		<xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after($data_type,'std::'))"/>&gt;
		</xsl:otherwise>
		</xsl:choose>
		</xsl:when>
		<xsl:when test="$data_type='int'"/>
		<xsl:when test="$data_type='short'"/>
		<xsl:when test="$data_type='long'"/>
		<xsl:when test="$data_type='char'"/>
		<xsl:when test="contains($data_type,'unsigned')"/>
		<xsl:when test="$data_type='bool'"/>
		<xsl:when test="$data_type='float'"/>
		<xsl:when test="$data_type='double'"/>
		<xsl:otherwise>
#include "<xsl:value-of select="$data_type"/>.h"
		</xsl:otherwise>
		</xsl:choose>
	</xsl:when>
	<xsl:otherwise>
		<xsl:variable name="nv" select="//ports/*[data_type=$data_type]/name"/>
		<xsl:if test="contains($nv,$data_name)">
		<xsl:choose>
		<xsl:when test="starts-with($data_type,'std::')">
		<xsl:choose>
		<xsl:when test="contains($data_type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after($data_type,'std::'),'&lt;'))"/>&gt;
		</xsl:when>
		<xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after($data_type,'std::'))"/>&gt;
		</xsl:otherwise>
		</xsl:choose>
		</xsl:when>
		<xsl:when test="$data_type='int'"/>
		<xsl:when test="$data_type='short'"/>
		<xsl:when test="$data_type='long'"/>
		<xsl:when test="$data_type='char'"/>
		<xsl:when test="contains($data_type,'unsigned')"/>
		<xsl:when test="$data_type='bool'"/>
		<xsl:when test="$data_type='float'"/>
		<xsl:when test="$data_type='double'"/>
		<xsl:otherwise>
#include "<xsl:value-of select="$data_type"/>.h"
		</xsl:otherwise>
		</xsl:choose>
		</xsl:if>
	</xsl:otherwise>
	</xsl:choose>
</xsl:for-each>
</xsl:if>

<xsl:if test="ports/service_port">
<xsl:for-each select="ports/service_port">
	<xsl:choose>
	<xsl:when test="usage='required'">
#include "<xsl:value-of select="type"/>Required.h"
	</xsl:when>
	<xsl:otherwise>
#include "<xsl:value-of select="type"/>Provided.h"
	</xsl:otherwise>
	</xsl:choose>
</xsl:for-each>
</xsl:if>


class <xsl:value-of select="name"/>: public Component<xsl:for-each select="ports/service_port[usage='provided']">
	,public<xsl:text> </xsl:text>I<xsl:value-of select="type"/>
</xsl:for-each>
{
protected:
// data
<xsl:for-each select="ports/data_port">
<xsl:choose>
<xsl:when test="usage='input'">
	InputDataPort&lt; <xsl:value-of select="data_type"/> &gt; <xsl:value-of select="name"/>;</xsl:when>
<xsl:otherwise>
	OutputDataPort&lt; <xsl:value-of select="data_type"/> &gt; <xsl:value-of select="name"/>;</xsl:otherwise>
</xsl:choose>
</xsl:for-each>

//event
<xsl:for-each select="ports/event_port">
<xsl:choose>
<xsl:when test="usage='input'">
	InputEventPort&lt; <xsl:value-of select="data_type"/> &gt; <xsl:value-of select="name"/>;</xsl:when>
<xsl:otherwise>
	OutputEventPort&lt; <xsl:value-of select="data_type"/> &gt; <xsl:value-of select="name"/>;</xsl:otherwise>
</xsl:choose>
</xsl:for-each>

// method for provider
	<xsl:for-each select="ports/service_port[usage='provided']">
	I<xsl:value-of select="type"/><xsl:text> *ptr</xsl:text><xsl:value-of select="name"/>;
</xsl:for-each>

// method for required
	<xsl:for-each select="ports/service_port[usage='required']">
	<xsl:value-of select="type"/>Required<xsl:text> *ptr</xsl:text><xsl:value-of select="name"/>;
</xsl:for-each>


// symbol value generation
	<xsl:if test="exports/var">
	<xsl:for-each select="exports/var">
	<xsl:value-of select="@type"/> m_<xsl:value-of select="@name"/>;
	</xsl:for-each>
	</xsl:if>

public:
	<xsl:value-of select="name"/>();
	<xsl:value-of select="name"/>(const std::string &amp;compId);
	virtual ~<xsl:value-of select="name"/>();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();

<xsl:for-each select="ports/service_port[usage='provided']">
<xsl:variable name="t" select="document(reference)"/>
<xsl:variable name="s_type" select="type"/>
<xsl:choose>
<xsl:when test="$t/service_port_type_profile/service_port_type/type_name=type">
<xsl:for-each select="$t/service_port_type_profile/service_port_type[type_name=$s_type]/method">
	virtual <xsl:value-of select="@return_type"/><xsl:text> </xsl:text><xsl:value-of select="@name"/>(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/></xsl:for-each>);</xsl:for-each>
</xsl:when>
<xsl:otherwise>
	&lt;error&gt; code generation error &lt;/error&gt;
	&lt;error&gt; Please Check Your Component Profile or Service Port Profile&lt;/error&gt; 
	&lt;error&gt; Profile name or profile type mismatch&lt;/error&gt; 
</xsl:otherwise>
</xsl:choose>

</xsl:for-each>
};

#endif

</xsl:template>
<!--
** Copyright (C) 2011 ETRI, All rights reserved
**  
**  
** This programs is the production of ETRI research & development activity;
** you cannot use it and cannot redistribute it and cannot modify it and
** cannnot  store it in any media(disk, photo or otherwise) without the prior
** permission of ETRI.
** 
** You should have received the license for this program to use any purpose.
** If not, contact the Electronics and Telecommunications Research Institute
** [ETRI], DaeDog DanJi, TaeJeon, 305-350, Korea.
** 
** NO PART OF THIS WORK BY THE THIS COPYRIGHT HEREON MAY BE REPRODUCED, STORED
** IN RETRIEVAL SYSTENS, IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
** PHOTOCOPYING, RECORDING OR OTHERWISE, WITHOUT THE PRIOR PERMISSION OF ETRI
**
** @Author: sby (sby@etri.re.kr)
-->
<xsl:template name="componentSource" match="component">
<xsl:param name="filename"/>
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
 *  
 */
#include &lt;Component.h&gt;
#include &lt;InputDataPort.h&gt;
#include &lt;OutputDataPort.h&gt;
#include &lt;InputEventPort.h&gt;
#include &lt;OutputEventPort.h&gt;
#include &lt;OPRoSTypes.h&gt;
#include &lt;EventData.h&gt;
#include &lt;ServiceUtils.h&gt;

<xsl:if test="ports/service_port">
<xsl:for-each select="ports/service_port">
	<xsl:choose>
	<xsl:when test="usage='required'">
#include "<xsl:value-of select="type"/>Required.h"
	</xsl:when>
	<xsl:otherwise>
#include "<xsl:value-of select="type"/>Provided.h"
	</xsl:otherwise>
	</xsl:choose>
</xsl:for-each>
</xsl:if>

#include "<xsl:value-of select="$filename"/>.h"

//
// constructor declaration
//
<xsl:value-of select="name"/>::<xsl:value-of select="name"/>()<xsl:if test="ports/data_port"><xsl:if test="ports/data_port/usage='input'">
		:<xsl:for-each select="ports/data_port[usage='input']"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="name"/>(<xsl:choose><xsl:when test="queueing_policy">OPROS_<xsl:value-of select="queueing_policy"/>,<xsl:value-of select="queue_size"/></xsl:when><xsl:otherwise>OPROS_LAST,1</xsl:otherwise></xsl:choose>)</xsl:for-each>
</xsl:if></xsl:if>
{
<xsl:for-each select="ports/service_port"><xsl:choose><xsl:when test="usage='provided'">
	</xsl:when><xsl:otherwise>	ptr<xsl:value-of select="name"/> = NULL;</xsl:otherwise></xsl:choose></xsl:for-each>
	
	portSetup();
}

//
// constructor declaration (with name)
//
<xsl:value-of select="name"/>::<xsl:value-of select="name"/>(const std::string &amp;name):Component(name)<xsl:if test="ports/data_port"><xsl:if test="ports/data_port/usage='input'">
		,<xsl:for-each select="ports/data_port[usage='input']"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="name"/>(<xsl:choose><xsl:when test="queueing_policy">OPROS_<xsl:value-of select="queueing_policy"/>,<xsl:value-of select="queue_size"/></xsl:when><xsl:otherwise>OPROS_LAST,1</xsl:otherwise></xsl:choose>)</xsl:for-each>
</xsl:if></xsl:if>
{
<xsl:for-each select="ports/service_port"><xsl:choose><xsl:when test="usage='provided'">
	</xsl:when><xsl:otherwise>	ptr<xsl:value-of select="name"/> = NULL;</xsl:otherwise></xsl:choose></xsl:for-each>
	
	portSetup();
}

//
// destructor declaration
//

<xsl:value-of select="name"/>::~<xsl:value-of select="name"/>() {
}

void <xsl:value-of select="name"/>::portSetup() {
<xsl:if test="ports/service_port">
<xsl:if test="ports/service_port/usage='provided'">
	// provided service port setup
	<xsl:for-each select="ports/service_port[usage='provided']">
	ProvidedServicePort *pa<xsl:value-of select="position()"/>;

	pa<xsl:value-of select="position()"/>=new <xsl:value-of select="type"/>Provided(this);

	addPort("<xsl:value-of select="name"/>",pa<xsl:value-of select="position()"/>);
</xsl:for-each>
</xsl:if>
<xsl:if test="ports/service_port/usage='required'">
	// required service port setup
<xsl:for-each select="ports/service_port[usage='required']">
	ptr<xsl:value-of select="name"/>=new <xsl:value-of select="type"/>Required();
	addPort("<xsl:value-of select="name"/>",ptr<xsl:value-of select="name"/>);
</xsl:for-each>
</xsl:if>
</xsl:if>

<xsl:if test="ports/data_port">
	// data port setup
<xsl:for-each select="ports/data_port">
<xsl:choose>
<xsl:when test="usage='input'">
	addPort("<xsl:value-of select="name"/>", &amp;<xsl:value-of select="name"/>);
</xsl:when>
<xsl:otherwise>
	addPort("<xsl:value-of select="name"/>", &amp;<xsl:value-of select="name"/>);
</xsl:otherwise>
</xsl:choose>
</xsl:for-each>
</xsl:if>

<xsl:if test="ports/event_port">
	// event port setup
<xsl:for-each select="ports/event_port">
<xsl:choose>
<xsl:when test="usage='input'">
	addPort("<xsl:value-of select="name"/>", &amp;<xsl:value-of select="name"/>);
	<xsl:value-of select="name"/>.setOwner(this);
</xsl:when>
<xsl:otherwise>
	addPort("<xsl:value-of select="name"/>", &amp;<xsl:value-of select="name"/>);
</xsl:otherwise>
</xsl:choose>
</xsl:for-each>
</xsl:if>


	// export variable setup
<xsl:if test="exports/var">
<xsl:for-each select="exports/var">
	EXPORT_VARIABLE("<xsl:value-of select="@name"/>",m_<xsl:value-of select="@name"/>);
</xsl:for-each>
</xsl:if>

}

// Call back Declaration
ReturnType <xsl:value-of select="name"/>::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType <xsl:value-of select="name"/>::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType <xsl:value-of select="name"/>::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType <xsl:value-of select="name"/>::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}



<xsl:variable name="cname" select="name"/>
<xsl:for-each select="ports/service_port[usage='provided']">
<xsl:variable name="t" select="document(reference)"/>
<xsl:variable name="s_type" select="type"/>
<xsl:if test="$t/service_port_type_profile/service_port_type/type_name=type">
	<xsl:for-each select="$t/service_port_type_profile/service_port_type[type_name=$s_type]/method">
	<xsl:value-of select="@return_type"/><xsl:text> </xsl:text><xsl:value-of select="$cname"/>::<xsl:value-of select="@name"/>(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/></xsl:for-each>)
{
	// user code here
}
</xsl:for-each>
</xsl:if>
</xsl:for-each>



#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new <xsl:value-of select="name"/>();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new <xsl:value-of select="name"/>();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

</xsl:template>
<!--
** Copyright (C) 2011 ETRI, All rights reserved
**  
**  
** This programs is the production of ETRI research & development activity;
** you cannot use it and cannot redistribute it and cannot modify it and
** cannnot  store it in any media(disk, photo or otherwise) without the prior
** permission of ETRI.
** 
** You should have received the license for this program to use any purpose.
** If not, contact the Electronics and Telecommunications Research Institute
** [ETRI], DaeDog DanJi, TaeJeon, 305-350, Korea.
** 
** NO PART OF THIS WORK BY THE THIS COPYRIGHT HEREON MAY BE REPRODUCED, STORED
** IN RETRIEVAL SYSTENS, IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
** PHOTOCOPYING, RECORDING OR OTHERWISE, WITHOUT THE PRIOR PERMISSION OF ETRI
**
** @Author: sby (sby@etri.re.kr)
-->

<!-- xsl:template name="dataPortHeader" match="struct"-->
<xsl:template name="dataPortHeader">
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0 [Symbol])
 *  
 */
#ifndef _<xsl:value-of select="@name"/>_DATATYPE_H
#define _<xsl:value-of select="@name"/>_DATATYPE_H
#include &lt;archive/opros_archive.h&gt;

<xsl:for-each select="item">
   <xsl:choose>
      <xsl:when test="starts-with(@type,'std::')">
         <xsl:choose>
            <xsl:when test="contains(@type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after(@type,'std::'),'&lt;'))"/>&gt;
            </xsl:when>
            <xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after(@type,'std::'))"/>&gt;
            </xsl:otherwise>
         </xsl:choose>
       </xsl:when>
       <xsl:when test="@type='int'"/>
       <xsl:when test="@type='short'"/>
       <xsl:when test="@type='long'"/>
       <xsl:when test="@type='char'"/>
       <xsl:when test="contains(@type,'unsigned')"/>
       <xsl:when test="@type='bool'"/>
       <xsl:when test="@type='float'"/>
       <xsl:when test="@type='double'"/>
       <xsl:otherwise>
#include "<xsl:value-of select="@type"/>.h"
       </xsl:otherwise>
    </xsl:choose>
</xsl:for-each>

/*
** 
** 
*/
class <xsl:value-of select="@name"/>
{
protected:
	friend class opros::archive::access;
	<xsl:for-each select="item"><xsl:value-of select="@type"/><xsl:text> </xsl:text>m_<xsl:value-of select="current()"/>;
	</xsl:for-each>
public:
	void save(opros::archive::oarchive &amp;ar, const unsigned int) 
	{
		<xsl:for-each select="item">ar &lt;&lt; m_<xsl:value-of select="current()"/>;
		</xsl:for-each>

	}

	void load(opros::archive::iarchive &amp;ar, const unsigned int)
	{
		<xsl:for-each select="item">ar &gt;&gt; m_<xsl:value-of select="current()"/>;
		</xsl:for-each>

	}
public:
	<xsl:value-of select="@name"/>()
	{
	}

	<xsl:value-of select="@name"/>(<xsl:for-each select="item"><xsl:if test="position()!=1">,</xsl:if>const <xsl:value-of select="@type"/>&amp;<xsl:text> </xsl:text>in_<xsl:value-of select="current()"/></xsl:for-each>)
	{
		<xsl:for-each select="item">m_<xsl:value-of select="current()"/> = in_<xsl:value-of select="current()"/>;
		</xsl:for-each>
	}

	<xsl:for-each select="item">
	<xsl:value-of select="@type"/>&amp;<xsl:text> </xsl:text>get<xsl:value-of select="translate(substring(current(),1,1), 'abcedefghijklmnopqrstuvwxyz','ABCEDEFGHIJKLMNOPQRSTUVWXYZ')"/><xsl:value-of select="substring(current(),2)"/>()
	{
		return m_<xsl:value-of select="current()"/>;
	}

	void set<xsl:value-of select="translate(substring(current(),1,1), 'abcedefghijklmnopqrstuvwxyz','ABCEDEFGHIJKLMNOPQRSTUVWXYZ')"/><xsl:value-of select="substring(current(),2)"/>(const <xsl:value-of select="@type"/>&amp;<xsl:text> </xsl:text>in_<xsl:value-of select="current()"/>)
	{
		m_<xsl:value-of select="current()"/> = in_<xsl:value-of select="current()"/>;
	}

	</xsl:for-each>

};

inline std::string typeName(<xsl:value-of select="@name"/> *p)
{
	return "<xsl:value-of select="@name"/>";
}
#endif
</xsl:template>
<!--
** Copyright (C) 2011 ETRI, All rights reserved
**  
**  
** This programs is the production of ETRI research & development activity;
** you cannot use it and cannot redistribute it and cannot modify it and
** cannnot  store it in any media(disk, photo or otherwise) without the prior
** permission of ETRI.
** 
** You should have received the license for this program to use any purpose.
** If not, contact the Electronics and Telecommunications Research Institute
** [ETRI], DaeDog DanJi, TaeJeon, 305-350, Korea.
** 
** NO PART OF THIS WORK BY THE THIS COPYRIGHT HEREON MAY BE REPRODUCED, STORED
** IN RETRIEVAL SYSTENS, IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
** PHOTOCOPYING, RECORDING OR OTHERWISE, WITHOUT THE PRIOR PERMISSION OF ETRI
**
** @Author: sby (sby@etri.re.kr)
-->

<!-- xsl:template name="methodPortHeader" match="service_port_type_profile" -->
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0 [Symbol])
 *  
 */
<!-- xsl:apply-templates select="service_port_type"/ -->
<!--/xsl:template -->
<xsl:template name="methodPortHeader"> <!-- //match="service_port_type" -->
<xsl:param name="portType"/>
<xsl:choose>
	<xsl:when test="$portType='provided'">
		<xsl:call-template name="ProvidedTemplate"/>
	</xsl:when>
	<xsl:otherwise>
		<xsl:call-template name="RequiredTemplate"/>
	</xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template name="RequiredTemplate">
<xsl:variable name="className" select="type_name"/>

#ifndef _<xsl:value-of select="$className"/>_REQUIRED_PORT_H
#define _<xsl:value-of select="$className"/>_REQUIRED_PORT_H

#include &lt;OPRoSTypes.h&gt;
#include &lt;InputDataPort.h&gt;
#include &lt;OutputDataPort.h&gt;
#include &lt;InputEventPort.h&gt;
#include &lt;OutputEventPort.h&gt;
#include &lt;ProvidedServicePort.h&gt;
#include &lt;RequiredServicePort.h&gt;
#include &lt;ProvidedMethod.h&gt;
#include &lt;RequiredMethod.h&gt;

<xsl:if test="method/param/type">
<xsl:for-each select="method/param">
	<xsl:variable name="m_name" select="../@name"/>
	<xsl:variable name="t_name" select="name"/>
	<xsl:variable name="t_type" select="type"/>
	<xsl:choose>
	<xsl:when test="count(//method/param/type[.=$t_type])=1">
		<xsl:choose>
		<xsl:when test="starts-with($t_type,'std::')">
		<xsl:choose>
		<xsl:when test="contains($t_type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after($t_type,'std::'),'&lt;'))"/>&gt;
		</xsl:when>
		<xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after($t_type,'std::'))"/>&gt;
		</xsl:otherwise>
		</xsl:choose>
		</xsl:when>
		<xsl:when test="$t_type='int'"/>
		<xsl:when test="$t_type='short'"/>
		<xsl:when test="$t_type='long'"/>
		<xsl:when test="$t_type='char'"/>
		<xsl:when test="contains($t_type,'unsigned')"/>
		<xsl:when test="$t_type='bool'"/>
		<xsl:when test="$t_type='float'"/>
		<xsl:when test="$t_type='double'"/>
		<xsl:otherwise>
#include "<xsl:value-of select="$t_type"/>.h"
		</xsl:otherwise>
		</xsl:choose>
	</xsl:when>
	<xsl:otherwise>
	<xsl:variable name="nv" select="//method/param[type=$t_type]/name"/>
	<xsl:if test="contains($nv,$t_name)">
		<xsl:choose>
		<xsl:when test="starts-with($t_type,'std::')">
		<xsl:choose>
		<xsl:when test="contains($t_type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after($t_type,'std::'),'&lt;'))"/>&gt;
		</xsl:when>
		<xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after($t_type,'std::'))"/>&gt;
		</xsl:otherwise>
		</xsl:choose>
		</xsl:when>
		<xsl:when test="$t_type='int'"/>
		<xsl:when test="$t_type='short'"/>
		<xsl:when test="$t_type='long'"/>
		<xsl:when test="$t_type='char'"/>
		<xsl:when test="contains($t_type,'unsigned')"/>
		<xsl:when test="$t_type='bool'"/>
		<xsl:when test="$t_type='float'"/>
		<xsl:when test="$t_type='double'"/>
		<xsl:otherwise>
#include "<xsl:value-of select="$t_type"/>.h"
		</xsl:otherwise>
		</xsl:choose>
	</xsl:if>
	</xsl:otherwise>
	</xsl:choose>
</xsl:for-each>
</xsl:if>



/*
 * 
 * <xsl:value-of select="$className"/> : public RequiredServicePort
 *
 */
class <xsl:value-of select="$className"/>Required
   :public RequiredServicePort
{
protected:
<xsl:for-each select="method">
	typedef RequiredMethod&lt;<xsl:value-of select="@return_type"/>&gt; <xsl:value-of select="@name"/>FuncType;
	<xsl:value-of select="@name"/>FuncType *<xsl:value-of select="@name"/>Func;
</xsl:for-each>
public:
	//
	// Constructor
	//
	<xsl:value-of select="$className"/>Required()
	{
            <xsl:for-each select="method"> <xsl:value-of select="@name"/>Func = NULL;
            </xsl:for-each>
	   setup();
	}

	// method implementation for required method
	<xsl:for-each select="method">
        <xsl:value-of select="@return_type"/><xsl:text> </xsl:text>
        <xsl:value-of select="@name"/>(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/></xsl:for-each>)
	{
            opros_assert(<xsl:value-of select="@name"/>Func != NULL);
	<xsl:choose>
		<xsl:when test="@return_type='void'">
            (*<xsl:value-of select="@name"/>Func)(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="name"/></xsl:for-each>);
		</xsl:when>
		<xsl:otherwise>
            return (*<xsl:value-of select="@name"/>Func)(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="name"/></xsl:for-each>);
		</xsl:otherwise>
	</xsl:choose>
	}

	</xsl:for-each>

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    <xsl:for-each select="method">
        ptr_method = makeRequiredMethod(&amp;<xsl:value-of select="$className"/>Required::<xsl:value-of select="@name"/>,"<xsl:value-of select="@name"/>");
        opros_assert(ptr_method != NULL);
        addMethod("<xsl:value-of select="@name"/>",ptr_method);
        <xsl:value-of select="@name"/>Func = reinterpret_cast&lt;<xsl:value-of select="@name"/>FuncType *&gt;(ptr_method);
        ptr_method = NULL;

    </xsl:for-each>
    }
};
#endif
</xsl:template>

<xsl:template name="ProvidedTemplate">
<xsl:variable name="className" select="type_name"/>
<xsl:variable name="providerName" select="concat('I',type_name)"/>

#ifndef _<xsl:value-of select="$className"/>_PROVIDED_PORT_H
#define _<xsl:value-of select="$className"/>_PROVIDED_PORT_H

#include &lt;OPRoSTypes.h&gt;
#include &lt;InputDataPort.h&gt;
#include &lt;OutputDataPort.h&gt;
#include &lt;InputEventPort.h&gt;
#include &lt;OutputEventPort.h&gt;
#include &lt;ProvidedServicePort.h&gt;
#include &lt;RequiredServicePort.h&gt;
#include &lt;ProvidedMethod.h&gt;
#include &lt;RequiredMethod.h&gt;

<xsl:if test="method/param/type">
<xsl:for-each select="method/param">
	<xsl:variable name="m_name" select="../@name"/>
	<xsl:variable name="t_name" select="name"/>
	<xsl:variable name="t_type" select="type"/>
	<xsl:choose>
	<xsl:when test="count(//method/param/type[.=$t_type])=1">
		<xsl:choose>
		<xsl:when test="starts-with($t_type,'std::')">
		<xsl:choose>
		<xsl:when test="contains($t_type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after($t_type,'std::'),'&lt;'))"/>&gt;
		</xsl:when>
		<xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after($t_type,'std::'))"/>&gt;
		</xsl:otherwise>
		</xsl:choose>
		</xsl:when>
		<xsl:when test="$t_type='int'"/>
		<xsl:when test="$t_type='short'"/>
		<xsl:when test="$t_type='long'"/>
		<xsl:when test="$t_type='char'"/>
		<xsl:when test="contains($t_type,'unsigned')"/>
		<xsl:when test="$t_type='bool'"/>
		<xsl:when test="$t_type='float'"/>
		<xsl:when test="$t_type='double'"/>
		<xsl:otherwise>
#include "<xsl:value-of select="$t_type"/>.h"
		</xsl:otherwise>
		</xsl:choose>
	</xsl:when>
	<xsl:otherwise>
	<xsl:variable name="nv" select="//method/param[type=$t_type]/name"/>
	<xsl:if test="contains($nv,$t_name)">
		<xsl:choose>
		<xsl:when test="starts-with($t_type,'std::')">
		<xsl:choose>
		<xsl:when test="contains($t_type,'&lt;')">
#include &lt;<xsl:value-of select="normalize-space(substring-before(substring-after($t_type,'std::'),'&lt;'))"/>&gt;
		</xsl:when>
		<xsl:otherwise>
#include &lt;<xsl:value-of select="normalize-space(substring-after($t_type,'std::'))"/>&gt;
		</xsl:otherwise>
		</xsl:choose>
		</xsl:when>
		<xsl:when test="$t_type='int'"/>
		<xsl:when test="$t_type='short'"/>
		<xsl:when test="$t_type='long'"/>
		<xsl:when test="$t_type='char'"/>
		<xsl:when test="contains($t_type,'unsigned')"/>
		<xsl:when test="$t_type='bool'"/>
		<xsl:when test="$t_type='float'"/>
		<xsl:when test="$t_type='double'"/>
		<xsl:otherwise>
#include "<xsl:value-of select="$t_type"/>.h"
		</xsl:otherwise>
		</xsl:choose>
	</xsl:if>
	</xsl:otherwise>
	</xsl:choose>
</xsl:for-each>
</xsl:if>

#ifndef _<xsl:value-of select="$providerName"/>_CLASS_DEF
#define _<xsl:value-of select="$providerName"/>_CLASS_DEF
/*
 * <xsl:value-of select="$providerName"/>
 *
 *  The comonent inherits this class and implements methods. 
 */
class <xsl:value-of select="$providerName"/>
{
 public:<xsl:for-each select="method">
    virtual <xsl:value-of select="@return_type"/><xsl:text> </xsl:text><xsl:value-of select="@name"/>(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/></xsl:for-each>)=0;</xsl:for-each>
 };
#endif

/*
 * 
 * <xsl:value-of select="$className"/> : public ProvidedServicePort
 *
 */
class <xsl:value-of select="$className"/>Provided
	:public ProvidedServicePort, public <xsl:value-of select="$providerName"/>
{
protected:
    <xsl:value-of select="$providerName"/> *pcom;

<xsl:for-each select="method">
   typedef ProvidedMethod&lt;<xsl:value-of select="@return_type"/>&gt; <xsl:value-of select="@name"/>FuncType;
   <xsl:value-of select="@name"/>FuncType *<xsl:value-of select="@name"/>Func;
</xsl:for-each>

public: // default method<xsl:for-each select="method">
   virtual <xsl:value-of select="@return_type"/><xsl:text> </xsl:text><xsl:value-of select="@name"/>(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/></xsl:for-each>)
   {
		opros_assert(<xsl:value-of select="@name"/>Func != NULL);
		<xsl:choose>
			<xsl:when test="@return_type='void'">(*<xsl:value-of select="@name"/>Func)(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="name"/></xsl:for-each>);</xsl:when>
		<xsl:otherwise>
            return (*<xsl:value-of select="@name"/>Func)(<xsl:for-each select="param"><xsl:if test="position()!=1">,</xsl:if><xsl:value-of select="name"/></xsl:for-each>);
		</xsl:otherwise>
	</xsl:choose>
   }</xsl:for-each>


public:
    //
    // Constructor
    //
    <xsl:value-of select="$className"/>Provided(<xsl:value-of select="$providerName"/> *fn)
    {
        pcom = fn;

        <xsl:for-each select="method"> <xsl:value-of select="@name"/>Func = NULL;
        </xsl:for-each>

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    <xsl:for-each select="method">
        ptr_method = makeProvidedMethod(&amp;<xsl:value-of select="$providerName"/>::<xsl:value-of select="@name"/>,pcom,"<xsl:value-of select="@name"/>");

        opros_assert(ptr_method != NULL);
        addMethod("<xsl:value-of select="@name"/>",ptr_method);
        <xsl:value-of select="@name"/>Func = reinterpret_cast&lt;<xsl:value-of select="@name"/>FuncType *&gt;(ptr_method);
        ptr_method = NULL;

    </xsl:for-each>
    }
};
#endif
</xsl:template>

<!--
** Copyright (C) 2008 ETRI, All rights reserved
**  
**  
** This programs is the production of ETRI research & development activity;
** you cannot use it and cannot redistribute it and cannot modify it and
** cannnot  store it in any media(disk, photo or otherwise) without the prior
** permission of ETRI.
** 
** You should have received the license for this program to use any purpose.
** If not, contact the Electronics and Telecommunications Research Institute
** [ETRI], DaeDog DanJi, TaeJeon, 305-350, Korea.
** 
** NO PART OF THIS WORK BY THE THIS COPYRIGHT HEREON MAY BE REPRODUCED, STORED
** IN RETRIEVAL SYSTENS, IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
** PHOTOCOPYING, RECORDING OR OTHERWISE, WITHOUT THE PRIOR PERMISSION OF ETRI
**
** @Author: sby (sby@etri.re.kr)
-->
<xsl:template name="makeGenerator">
<xsl:param name="name"/>
<xsl:variable name="cfgmodule" select="document('libconfig.cfg')"/>
	<xsl:document href="project/Makefile.vc" method="text">
		<xsl:call-template name="makeGeneratorVS2008">
			<xsl:with-param name="name" select="$name"/>
			<xsl:with-param name="cfg" select="$cfgmodule"/>
		</xsl:call-template>
	</xsl:document>
	<xsl:document href="project/MakefileD.vc" method="text">
		<xsl:call-template name="makeGeneratorVS2008Debug">
			<xsl:with-param name="name" select="$name"/>
			<xsl:with-param name="cfg" select="$cfgmodule"/>
		</xsl:call-template>
	</xsl:document>
	<xsl:document href="project/Makefile.gcc" method="text">
		<xsl:call-template name="makeGeneratorGCC">
			<xsl:with-param name="name" select="$name"/>
			<xsl:with-param name="cfg" select="$cfgmodule"/>
		</xsl:call-template>
	</xsl:document>
	<xsl:document href="project/{$name}.vcproj" method="xml" indent="yes" encoding="euc-kr">
		<xsl:call-template name="makeGeneratorVCPROJ">
			<xsl:with-param name="name" select="$name"/>
			<xsl:with-param name="cfg" select="$cfgmodule"/>
		</xsl:call-template>
	</xsl:document>
</xsl:template>

<xsl:template name="makeGeneratorVS2008">
<xsl:param name="name"/>
<xsl:param name="cfg"/>
OUTDIR=.\Release
INTDIR=.\Release
OutDir=.\Release
<xsl:choose>
<xsl:when test="$cfg/libconfig">
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/boost_inc">
BOOST_INC=<xsl:value-of select="$cfg/libconfig/boost_inc"/>
	</xsl:when>
	<xsl:otherwise>
BOOST_INC=./boost/include/boost-1_38 # change this with your boost include path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/boost_lib">
BOOST_LIB=<xsl:value-of select="$cfg/libconfig/boost_lib"/>
	</xsl:when>
	<xsl:otherwise>
BOOST_LIB=./boost/lib                # change this with your boost lib path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/opros_inc">
OPROS_INC=<xsl:value-of select="$cfg/libconfig/opros_inc"/>
	</xsl:when>
	<xsl:otherwise>
OPROS_INC=./OPRoSLib/include         # change this with your opros include path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/opros_lib">
OPROS_LIB=<xsl:value-of select="$cfg/libconfig/opros_lib"/>
	</xsl:when>
	<xsl:otherwise>
OPROS_LIB=./OPRoSLib/lib             # change this with your opros lib path
	</xsl:otherwise>
  </xsl:choose>
</xsl:when>
<xsl:otherwise>
BOOST_INC=./boost/include/boost-1_38 # change this with your boost include path
BOOST_LIB=./boost/lib                # change this with your boost lib path
OPROS_INC=./OPRoSLib/include         # change this with your opros include path
OPROS_LIB=./OPRoSLib/lib             # change this with your opros lib path
</xsl:otherwise>
</xsl:choose>

PROJECT=<xsl:value-of select="$name"/>
TARGET= $(PROJECT).dll
OBJ1 =   $(PROJECT).obj
#OBJ2 =  some.obj

ALL: $(OUTDIR)\$(TARGET)

CLEAN: 
	-@erase "$(INTDIR)\$(TARGET)"
	-@erase "$(INTDIR)\$(OBJ1)"
	-@erase "$(INTDIR)\$(PROJECT).lib"
#	-@erase "$(INTDIR)\$(OBJ2)"
	
	

$(OUTDIR) :
	if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"
	
INC = /I "$(BOOST_INC)" /I "$(OPROS_INC)"

CPP=cl.exe

CPP_PROJ=/O2 /Oi $(INC) /GL /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" \
 /D "UNICODE" /FD /EHsc /MD /Gy /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /W0 /nologo /c /Zi /TP /errorReport:prompt

.cpp{$(INTDIR)}.obj::
	$(CPP) $(CPP_PROJ) $&lt; 

LINK32=link.exe
LINK32_FLAGS=/OUT:"$(OUTDIR)\$(TARGET)" /INCREMENTAL:NO /NOLOGO /libpath:"$(BOOST_LIB)" /libpath:"$(OPROS_LIB)" \
	/DLL /MANIFEST /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:PROMPT \
	OCECDL.lib 


LINK32_OBJS= \
	"$(INTDIR)\$(OBJ1)" 
#	"$(INTDIR)\$(OBJ2)"
	
	
$(OUTDIR)\$(TARGET) : $(OUTDIR) $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)	


</xsl:template>

<xsl:template name="makeGeneratorVS2008Debug">
<xsl:param name="name"/>
<xsl:param name="cfg"/>
OUTDIR=.\Debug
INTDIR=.\Debug
OutDir=.\Debug
<xsl:choose>
<xsl:when test="$cfg/libconfig">
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/boost_inc">
BOOST_INC=<xsl:value-of select="$cfg/libconfig/boost_inc"/>
	</xsl:when>
	<xsl:otherwise>
BOOST_INC=./boost/include/boost-1_38 # change this with your boost include path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/boost_lib">
BOOST_LIB=<xsl:value-of select="$cfg/libconfig/boost_lib"/>
	</xsl:when>
	<xsl:otherwise>
BOOST_LIB=./boost/lib                # change this with your boost lib path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/opros_inc">
OPROS_INC=<xsl:value-of select="$cfg/libconfig/opros_inc"/>
	</xsl:when>
	<xsl:otherwise>
OPROS_INC=./OPRoSLib/include         # change this with your opros include path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/opros_lib">
OPROS_LIB=<xsl:value-of select="$cfg/libconfig/opros_lib"/>
	</xsl:when>
	<xsl:otherwise>
OPROS_LIB=./OPRoSLib/lib             # change this with your opros lib path
	</xsl:otherwise>
  </xsl:choose>
</xsl:when>
<xsl:otherwise>
BOOST_INC=./boost/include/boost-1_38 # change this with your boost include path
BOOST_LIB=./boost/lib                # change this with your boost lib path
OPROS_INC=./OPRoSLib/include         # change this with your opros include path
OPROS_LIB=./OPRoSLib/lib             # change this with your opros lib path
</xsl:otherwise>
</xsl:choose>

PROJECT=<xsl:value-of select="$name"/>
TARGET= $(PROJECT).dll
OBJ1 =   $(PROJECT).obj
#OBJ2 =  some.obj

ALL: $(OUTDIR)\$(TARGET)

CLEAN: 
	-@erase "$(INTDIR)\$(TARGET)"
	-@erase "$(INTDIR)\$(OBJ1)"
	-@erase "$(INTDIR)\$(PROJECT).lib"
#	-@erase "$(INTDIR)\$(OBJ2)"
	
	

$(OUTDIR) :
	if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"
	
INC = /I "$(BOOST_INC)" /I "$(OPROS_INC)"

CPP=cl.exe

CPP_PROJ=/Od $(INC) /GL /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" \
/D "UNICODE" /Gm /EHsc /RTC1 /MDd /Gy /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /W3 /nologo /c /Zi /TP /errorReport:prompt

.cpp{$(INTDIR)}.obj::
	$(CPP) $(CPP_PROJ) $&lt; 

LINK32=link.exe
LINK32_FLAGS=/OUT:"$(OUTDIR)\$(TARGET)" /INCREMENTAL:NO /NOLOGO /libpath:"$(BOOST_LIB)" /libpath:"$(OPROS_LIB)" \
	/DLL /MANIFEST /DEBUG /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF /LTCG /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:PROMPT \
	OCECDLD.lib 


LINK32_OBJS= \
	"$(INTDIR)\$(OBJ1)" 
#	"$(INTDIR)\$(OBJ2)"
	
	
$(OUTDIR)\$(TARGET) : $(OUTDIR) $(DEF_FILE) $(LINK32_OBJS)
	$(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)	
</xsl:template>

<xsl:template name="makeGeneratorGCC">
<xsl:param name="name"/>
<xsl:param name="cfg"/>
OUTDIR=.\Release
INTDIR=.\Release
OutDir=.\Release
<xsl:choose>
<xsl:when test="$cfg/libconfig">
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/boost_inc">
BOOST_INC=<xsl:value-of select="$cfg/libconfig/boost_inc"/>
	</xsl:when>
	<xsl:otherwise>
BOOST_INC=./boost/include/boost-1_38 # change this with your boost include path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/boost_lib">
BOOST_LIB=<xsl:value-of select="$cfg/libconfig/boost_lib"/>
	</xsl:when>
	<xsl:otherwise>
BOOST_LIB=./boost/lib                # change this with your boost lib path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/opros_inc">
OPROS_INC=<xsl:value-of select="$cfg/libconfig/opros_inc"/>
	</xsl:when>
	<xsl:otherwise>
OPROS_INC=./OPRoSLib/include         # change this with your opros include path
	</xsl:otherwise>
  </xsl:choose>
  <xsl:choose>
	  <xsl:when test="$cfg/libconfig/opros_lib">
OPROS_LIB=<xsl:value-of select="$cfg/libconfig/opros_lib"/>
	</xsl:when>
	<xsl:otherwise>
OPROS_LIB=./OPRoSLib/lib             # change this with your opros lib path
	</xsl:otherwise>
  </xsl:choose>
</xsl:when>
<xsl:otherwise>
BOOST_INC=./boost/include/boost-1_38 # change this with your boost include path
BOOST_LIB=./boost/lib                # change this with your boost lib path
OPROS_INC=./OPRoSLib/include         # change this with your opros include path
OPROS_LIB=./OPRoSLib/lib             # change this with your opros lib path
</xsl:otherwise>
</xsl:choose>
SO_TYPE	= so                         # shared object extensions (so or dll)

PROJECT = <xsl:value-of select="$name"/>

INC = -I$(BOOST_INC) -I$(OPROS_INC)

CXXFLAGS = -O2 -Wall -fmessage-length=0 -mthreads -fPIC $(INC)

LFLAGS = -L$(BOOST_LIB) -L$(OPROS_LIB)

OBJS1 = $(PROJECT).o

LIBS = -loproscdl -lboost_iostreams-mgw34-mt -lboost_serialization-mgw34-mt

TARGET = $(PROJECT).$(SO_TYPE)

$(TARGET): $(OBJS1)
	$(CXX) $(LFLAGS) -shared -Wl,-soname,$@ -o $@ $(OBJS1) $(LIBS)

all: $(TARGET)

default: $(TARGET)

clean:
	del -f $(OBJS1) $(TARGET)
</xsl:template>
<!--
** Copyright (C) 2008 ETRI, All rights reserved
**  
**  
** This programs is the production of ETRI research & development activity;
** you cannot use it and cannot redistribute it and cannot modify it and
** cannnot  store it in any media(disk, photo or otherwise) without the prior
** permission of ETRI.
** 
** You should have received the license for this program to use any purpose.
** If not, contact the Electronics and Telecommunications Research Institute
** [ETRI], DaeDog DanJi, TaeJeon, 305-350, Korea.
** 
** NO PART OF THIS WORK BY THE THIS COPYRIGHT HEREON MAY BE REPRODUCED, STORED
** IN RETRIEVAL SYSTENS, IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
** PHOTOCOPYING, RECORDING OR OTHERWISE, WITHOUT THE PRIOR PERMISSION OF ETRI
**
** @Author: sby (sby@etri.re.kr)
-->

<xsl:template match="/">
	<xsl:variable name="module" select="document(concat($filename,'.xml'))"/>
	<xsl:apply-templates select="$module/*"/>
</xsl:template>
<xsl:template match="component">
	<xsl:document href="project/{concat(name,'.h')}" method="text">
	<xsl:call-template name="componentHeader">
		<xsl:with-param name="filename" select="name"/>
	</xsl:call-template>
	</xsl:document>

	<xsl:document href="project/{concat(name,'.cpp')}" method="text">
	<xsl:call-template name="componentSource">
		<xsl:with-param name="filename" select="name"/>
	</xsl:call-template>
	</xsl:document>
</xsl:template>
<xsl:template match="component_profile">
	<xsl:document href="project/{concat(name,'.h')}" method="text">
	<xsl:call-template name="componentHeader">
		<xsl:with-param name="filename" select="name"/>
	</xsl:call-template>
	</xsl:document>

	<xsl:document href="project/{concat(name,'.cpp')}" method="text">
	<xsl:call-template name="componentSource">
		<xsl:with-param name="filename" select="name"/>
	</xsl:call-template>
	</xsl:document>

	<xsl:call-template name="makeGenerator">
		<xsl:with-param name="name" select="name"/>
	</xsl:call-template>


	<xsl:document href="project/mfc/{concat(name,'.h')}" method="text">
#include "<xsl:value-of select="name"/>App.h"
	<xsl:call-template name="componentHeader">
		<xsl:with-param name="filename" select="name"/>
	</xsl:call-template>
	</xsl:document>

	<xsl:document href="project/mfc/{concat(name,'.cpp')}" method="text">
#include "stdafx.h"
	<xsl:call-template name="componentSource">
		<xsl:with-param name="filename" select="name"/>
	</xsl:call-template>
	</xsl:document>

	<xsl:call-template name="makeMFCGenerator">
		<xsl:with-param name="name" select="name"/>
	</xsl:call-template>

</xsl:template>

<xsl:template match="service_port_type">
	<xsl:choose>
	<xsl:when test="$usage='none'">
		<xsl:document href="project/include/{concat(type_name,'Provided.h')}" method="text">
			<xsl:call-template name="methodPortHeader">
				<xsl:with-param name="portType" select="'provided'"/>
			</xsl:call-template>
		</xsl:document>
		<xsl:document href="project/include/{concat(type_name,'Required.h')}" method="text">
			<xsl:call-template name="methodPortHeader">
				<xsl:with-param name="portType" select="'required'"/>
			</xsl:call-template>
		</xsl:document>
	</xsl:when>
	<xsl:when test="$usage='provided'">
		<xsl:document href="project/include/{concat(type_name,'Provided.h')}" method="text">
			<xsl:call-template name="methodPortHeader">
				<xsl:with-param name="portType" select="'provided'"/>
			</xsl:call-template>
		</xsl:document>
	</xsl:when>
	<xsl:when test="$usage='required'">
		<xsl:document href="project/include/{concat(type_name,'Required.h')}" method="text">
			<xsl:call-template name="methodPortHeader">
				<xsl:with-param name="portType" select="'required'"/>
			</xsl:call-template>
		</xsl:document>
	</xsl:when>
	<xsl:otherwise>
second parameter can be only 'provided' or 'required'
	</xsl:otherwise>
	</xsl:choose>
</xsl:template>
<xsl:template match="data_type">
<xsl:for-each select="struct">
	<xsl:document href="project/include/{concat(@name,'.h')}" method="text">
		<xsl:call-template name="dataPortHeader"/>
	</xsl:document>
</xsl:for-each>
</xsl:template>

<xsl:template name="makeGeneratorVCPROJ">
<xsl:param name="name"/>
<xsl:param name="cfg"/>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="{$name}"
	RootNamespace="{$name}"
	Keyword="Win32Proj"
	TargetFrameworkVersion="0"
	>
	<Platforms>
		<Platform
			Name="Win32"
		/>
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory="$(SolutionDir)$(ConfigurationName)"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="2"
			CharacterSet="1"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				AdditionalIncludeDirectories="{$cfg/libconfig/opros_inc};{$cfg/libconfig/boost_inc};.\include"
				PreprocessorDefinitions="WIN32;_DEBUG;_CONSOLE;"
				MinimalRebuild="true"
				BasicRuntimeChecks="3"
				RuntimeLibrary="3"
				UsePrecompiledHeader="0"
				WarningLevel="3"
				DebugInformationFormat="4"
				DisableSpecificWarnings="4290;4819;4996"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="OPRoSCDL.lib"
				LinkIncremental="2"
				AdditionalLibraryDirectories="{$cfg/libconfig/boost_lib};{$cfg/libconfig/opros_lib}"
				GenerateDebugInformation="true"
				SubSystem="1"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="$(SolutionDir)$(ConfigurationName)"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="2"
			CharacterSet="1"
			WholeProgramOptimization="1"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="2"
				EnableIntrinsicFunctions="true"
				AdditionalIncludeDirectories="{$cfg/libconfig/opros_inc};{$cfg/libconfig/boost_inc};.\include"
				PreprocessorDefinitions="WIN32;NDEBUG;_CONSOLE;"
				RuntimeLibrary="2"
				EnableFunctionLevelLinking="true"
				UsePrecompiledHeader="0"
				WarningLevel="3"
				DebugInformationFormat="3"
				DisableSpecificWarnings="4290;4819;4996"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="OPRoSCDL.lib"
				LinkIncremental="1"
				AdditionalLibraryDirectories="{$cfg/libconfig/boost_lib};{$cfg/libconfig/opros_lib}"
				GenerateDebugInformation="true"
				SubSystem="1"
				OptimizeReferences="2"
				EnableCOMDATFolding="2"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
	</Configurations>
	<References>
	</References>
	<Files>
		<Filter
			Name="source"
			Filter="cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
			>
			<File
				RelativePath=".\{$name}.cpp"
				>
			</File>
		</Filter>
		<Filter
			Name="header"
			Filter="h;hpp;hxx;hm;inl;inc;xsd"
			>
			<File
				RelativePath=".\{$name}.h"
				>
			</File>
		</Filter>
		<Filter
			Name="resource"
			Filter="rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav"
			>
		</Filter>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
</xsl:template>

<xsl:template name="makeMFCGenerator">
<xsl:param name="name"/>
<xsl:variable name="cfg" select="document('libconfig.cfg')"/>
<xsl:document href="project/mfc/Resource.h" method="text">//{{NO_DEPENDENCIES}}&#13;
// Microsoft Visual C++ generated include file.&#13;
// Used by <xsl:value-of select="$name"/>.rc&#13;
// &#13;
&#13;
// Next default values for new objects&#13;
//&#13;
#ifdef APSTUDIO_INVOKED&#13;
#ifndef APSTUDIO_READONLY_SYMBOLS&#13;
#define _APS_NEXT_RESOURCE_VALUE	20000&#13;
#define _APS_NEXT_COMMAND_VALUE		32771&#13;
#define _APS_NEXT_CONTROL_VALUE		20000&#13;
#define _APS_NEXT_SYMED_VALUE		20000&#13;
#endif&#13;
#endif&#13;
</xsl:document>
	<xsl:document href="project/mfc/stdafx.cpp" method="text">
// stdafx.cpp : 
// <xsl:value-of select="$name"/>.pch is pre-compiled header
// stdafx.obj

#include "stdafx.h"
	</xsl:document>
	<xsl:document href="project/mfc/stdafx.h" method="text">
// stdafx.h : 

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      

#include &lt;afxwin.h&gt;         
#include &lt;afxext.h&gt;        

#ifndef _AFX_NO_OLE_SUPPORT
#include &lt;afxole.h&gt;       
#include &lt;afxodlgs.h&gt;    
#include &lt;afxdisp.h&gt;    
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include &lt;afxdb.h&gt;     
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include &lt;afxdao.h&gt;   
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include &lt;afxdtctl.h&gt;
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include &lt;afxcmn.h&gt;              
#endif // _AFX_NO_AFXCMN_SUPPORT
	</xsl:document>
	<xsl:document href="project/mfc/targetver.h" method="text">
#pragma once

// 
// 
// 
// 

// 
// 
#ifndef WINVER                          // 
#define WINVER 0x0600           // 
#endif

#ifndef _WIN32_WINNT            // 
#define _WIN32_WINNT 0x0600     // 
#endif

#ifndef _WIN32_WINDOWS          // 
#define _WIN32_WINDOWS 0x0410 // 
#endif

#ifndef _WIN32_IE                       // 
#define _WIN32_IE 0x0700        // 
#endif
	</xsl:document>
	<xsl:document href="project/mfc/{$name}.def" method="text">
; <xsl:value-of select="$name"/>.def 

LIBRARY      "<xsl:value-of select="$name"/>"

EXPORTS
    ; 
	</xsl:document>
	<xsl:document href="project/mfc/{$name}.rc" method="text">// Microsoft Visual C++ generated resource script.
//
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#ifndef APSTUDIO_INVOKED
#include "targetver.h"
#endif
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// Korean resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_KOR)
#ifdef _WIN32
LANGUAGE LANG_KOREAN, SUBLANG_DEFAULT
#pragma code_page(949)
#endif //_WIN3

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE
BEGIN
    "resource.h\0"
END

2 TEXTINCLUDE
BEGIN
	"#ifndef APSTUDIO_INVOKED\r\n"
    "#include ""targetver.h""\r\n"
    "#endif\r\n"
    "#include ""afxres.h""\r\n"
    "\0"
END

3 TEXTINCLUDE
BEGIN
    "#define _AFX_NO_SPLITTER_RESOURCES\r\n"
    "#define _AFX_NO_OLE_RESOURCES\r\n"
    "#define _AFX_NO_TRACKER_RESOURCES\r\n"
    "#define _AFX_NO_PROPERTY_RESOURCES\r\n"
    "\r\n"
	"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_KOR)\r\n"
	"LANGUAGE 18, 1\r\n"
	"#pragma code_page(949)\r\n"
    "#include ""res\\<xsl:value-of select="$name"/>.rc2""  \r\n"
#ifndef _AFXDLL
    "#include ""l.KOR\\afxres.rc""  	\r\n"
#endif
    "#endif\r\n"
    "\0"
END

/////////////////////////////////////////////////////////////////////////////
#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO     VERSIONINFO
  FILEVERSION       1,0,0,1
  PRODUCTVERSION    1,0,0,1
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x4L
 FILETYPE 0x2L
 FILESUBTYPE 0x0L
BEGIN
	BLOCK "StringFileInfo"
	BEGIN
        BLOCK "041203b5"
		BEGIN
            VALUE "CompanyName", "TODO: &lt;Company Name&gt;"
            VALUE "FileDescription", "TODO: &lt;File Description&gt;"
			VALUE "FileVersion",     "1.0.0.1"
			VALUE "InternalName",    "<xsl:value-of select="$name"/>"
            VALUE "LegalCopyright", "TODO: (c) &lt;Company Name&gt;.  All rights reserved."
			VALUE "OriginalFilename","<xsl:value-of select="$name"/>"
            VALUE "ProductName", "TODO: &lt;Product Name&gt;"
			VALUE "ProductVersion",  "1.0.0.1"
		END
	END
	BLOCK "VarFileInfo"
	BEGIN
		VALUE "Translation", 0x0412, 949
    END
END

#endif	// Korean resources
/////////////////////////////////////////////////////////////////////////////



#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//
#define _AFX_NO_SPLITTER_RESOURCES
#define _AFX_NO_OLE_RESOURCES
#define _AFX_NO_TRACKER_RESOURCES
#define _AFX_NO_PROPERTY_RESOURCES

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_KOR)
LANGUAGE 18, 1
#pragma code_page(949)
#include "res\\<xsl:value-of select="$name"/>.rc2"
#include "l.KOR\\afxres.rc"
#endif

/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED

</xsl:document>
	<xsl:document href="project/mfc/res/{$name}.rc2" method="text">//
// {$name}.RC2 
//

#ifdef APSTUDIO_INVOKED
#error This file is not able to be edited in Microsoft Visual C++
#endif //APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
// 

/////////////////////////////////////////////////////////////////////////////
	</xsl:document>
	<xsl:document href="project/mfc/{$name}App.h" method="text">
// <xsl:value-of select="$name"/>.h : <xsl:value-of select="$name"/> DLL 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH this file."
#endif

#include "resource.h"		// 

// C<xsl:value-of select="$name"/>App
// 
//

class C<xsl:value-of select="$name"/>App : public CWinApp
{
public:
	C<xsl:value-of select="$name"/>App();

// 
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
	</xsl:document>
	<xsl:document href="project/mfc/{$name}App.cpp" method="text">
// <xsl:value-of select="$name"/>.cpp : 
//
#include "stdafx.h"
#include "<xsl:value-of select="$name"/>App.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFApp

BEGIN_MESSAGE_MAP(C<xsl:value-of select="$name"/>App, CWinApp)
END_MESSAGE_MAP()

// C<xsl:value-of select="$name"/>App creation

C<xsl:value-of select="$name"/>App::C<xsl:value-of select="$name"/>App()
{
	// 
	// 
}


// unique C<xsl:value-of select="$name"/>App object.

C<xsl:value-of select="$name"/>App theApp;


// initialize C<xsl:value-of select="$name"/>App 

BOOL C<xsl:value-of select="$name"/>App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
	</xsl:document>

	<xsl:document href="project/mfc/{$name}.vcproj" method="xml" indent="yes" encoding="euc-kr">
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"	
	Name="{$name}"
	RootNamespace="{$name}"
	Keyword="MFCDLLProj"
	TargetFrameworkVersion="131072"
	>
	<Platforms>
		<Platform
			Name="Win32"
		/>
	</Platforms>
	<ToolFiles>
	</ToolFiles>
	<Configurations>
		<Configuration
			Name="Debug|Win32"
			OutputDirectory="$(SolutionDir)$(ConfigurationName)"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="2"
			UseOfMFC="2"
			CharacterSet="1"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions="_DEBUG"
				MkTypLibCompatible="false"
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				AdditionalIncludeDirectories="{$cfg/libconfig/opros_inc};{$cfg/libconfig/boost_inc};..\include"
				PreprocessorDefinitions="WIN32;_WINDOWS;_DEBUG;_USRDLL;"
				MinimalRebuild="true"
				BasicRuntimeChecks="3"
				RuntimeLibrary="3"
				UsePrecompiledHeader="2"
				WarningLevel="3"
				DebugInformationFormat="4"
				DisableSpecificWarnings="4290;4819;4996"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="_DEBUG"
				Culture="1042"
				AdditionalIncludeDirectories="$(IntDir)"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="OPRoSCDL.lib"
				LinkIncremental="2"
				AdditionalLibraryDirectories="{$cfg/libconfig/boost_lib};{$cfg/libconfig/opros_lib}"
				ModuleDefinitionFile=".\{$name}.def"
				GenerateDebugInformation="true"
				SubSystem="2"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
		<Configuration
			Name="Release|Win32"
			OutputDirectory="$(SolutionDir)$(ConfigurationName)"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="2"
			UseOfMFC="2"
			CharacterSet="1"
			WholeProgramOptimization="1"
			>
			<Tool
				Name="VCPreBuildEventTool"
			/>
			<Tool
				Name="VCCustomBuildTool"
			/>
			<Tool
				Name="VCXMLDataGeneratorTool"
			/>
			<Tool
				Name="VCWebServiceProxyGeneratorTool"
			/>
			<Tool
				Name="VCMIDLTool"
				PreprocessorDefinitions="NDEBUG"
				MkTypLibCompatible="false"
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="2"
				EnableIntrinsicFunctions="true"
				AdditionalIncludeDirectories="{$cfg/libconfig/opros_inc};{$cfg/libconfig/boost_inc};..\include"
				PreprocessorDefinitions="WIN32;_WINDOWS;NDEBUG;_USRDLL;"
				RuntimeLibrary="2"
				EnableFunctionLevelLinking="true"
				UsePrecompiledHeader="2"
				WarningLevel="3"
				DebugInformationFormat="3"
				DisableSpecificWarnings="4290;4819;4996"
			/>
			<Tool
				Name="VCManagedResourceCompilerTool"
			/>
			<Tool
				Name="VCResourceCompilerTool"
				PreprocessorDefinitions="NDEBUG"
				Culture="1042"
				AdditionalIncludeDirectories="$(IntDir)"
			/>
			<Tool
				Name="VCPreLinkEventTool"
			/>
			<Tool
				Name="VCLinkerTool"
				AdditionalDependencies="OPRoSCDL.lib"
				LinkIncremental="1"
				AdditionalLibraryDirectories="{$cfg/libconfig/boost_lib};{$cfg/libconfig/opros_lib}"
				ModuleDefinitionFile=".\{$name}.def"
				GenerateDebugInformation="true"
				SubSystem="2"
				OptimizeReferences="2"
				EnableCOMDATFolding="2"
				TargetMachine="1"
			/>
			<Tool
				Name="VCALinkTool"
			/>
			<Tool
				Name="VCManifestTool"
			/>
			<Tool
				Name="VCXDCMakeTool"
			/>
			<Tool
				Name="VCBscMakeTool"
			/>
			<Tool
				Name="VCFxCopTool"
			/>
			<Tool
				Name="VCAppVerifierTool"
			/>
			<Tool
				Name="VCPostBuildEventTool"
			/>
		</Configuration>
	</Configurations>
	<References>
	</References>
	<Files>
		<Filter
			Name="source file"
			Filter="cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
			>
			<File
				RelativePath=".\stdafx.cpp"
				>
				<FileConfiguration
					Name="Debug|Win32"
					>
					<Tool
						Name="VCCLCompilerTool"
						UsePrecompiledHeader="1"
					/>
				</FileConfiguration>
				<FileConfiguration
					Name="Release|Win32"
					>
					<Tool
						Name="VCCLCompilerTool"
						UsePrecompiledHeader="1"
					/>
				</FileConfiguration>
			</File>
			<File
				RelativePath=".\{$name}App.cpp"
				>
			</File>
			<File
				RelativePath=".\{$name}.cpp"
				>
			</File>
			<File
				RelativePath=".\{$name}.def"
				>
			</File>
		</Filter>
		<Filter
			Name="header file"
			Filter="h;hpp;hxx;hm;inl;inc;xsd"
			>
			<File
				RelativePath=".\Resource.h"
				>
			</File>
			<File
				RelativePath=".\stdafx.h"
				>
			</File>
			<File
				RelativePath=".\targetver.h"
				>
			</File>
			<File
				RelativePath=".\{$name}.h"
				>
			</File>
			<File
				RelativePath=".\{$name}App.h"
				>
			</File>
		</Filter>
		<Filter
			Name="Resource File"
			Filter="rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx;tiff;tif;png;wav"
			>
			<File
				RelativePath=".\{$name}.rc"
				>
			</File>
			<File
				RelativePath=".\res\{$name}.rc2"
				>
			</File>
		</Filter>
		<File
			RelativePath=".\ReadMe.txt"
			>
		</File>
	</Files>
	<Globals>
	</Globals>
</VisualStudioProject>
	</xsl:document>
</xsl:template>
</xsl:stylesheet>
