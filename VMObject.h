#pragma once
#include <string>
#include "SMObject.h"
#include "ServerMachine.h"
#include "VirtualMachine.h"

class SMObject;

//������ڵ�����ö��
enum class VM_NodeType
{
	Unknow,
	A,
	B,
	Both
};

/// <summary>
/// �÷���
/// 1.���������ʱ��Ҫ�ڹ��캯���д������������������nullptr����Ҫʹ��SetFather��������
/// 2.ʹ��ChangeFather���Դ�ԭ��������ɾ���Լ���Ȼ����Լ���ӽ��·�����
/// 3.ʹ��LeaveFather���Դ�ԭ��������ɾ���Լ�
/// </summary>
class VMObject
{
private:
	const VirtualMachine& VMProperty; //����
	int VM_ID;				   //�����ID
	VM_NodeType nodeType;	   //������ڵ�����
	SMObject* father;		   //���Ĵ�ķ�����

public:
	friend class SMObject;
	//���캯�������ԣ�VM�ţ�������������
	VMObject(const VirtualMachine& _VMProperty, int _vm_id, SMObject* _father);

	~VMObject();
	

	//��ʼ��father��������father��᷵��false
	bool SetFather(SMObject* _father);
	

	//�޸ļĴ�ķ����������ԭ��������ɾ���Լ��������ԭ�������Ļ�����
	bool ChangeFather(SMObject* _newfather);

	//��ԭ���������뿪
	bool LeaveFather();

	VM_NodeType GetNodeType() const { return nodeType; } //��ȡ�ڵ�����
	void SetNodeType(VM_NodeType _nodeType) { nodeType = _nodeType; }
	const VirtualMachine& GetProperty() const { return VMProperty; } //��ȡ����
	int GetID() const { return VM_ID; }						  //��ȡVM_ID
	int GetFatherID() const;
};
