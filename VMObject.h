#pragma once
#include <string>
#include "SMObject.h"
#include "ServerMachine.h"
#include "VirtualMachine.h"

//������ڵ�����ö��
enum VM_NodeType { Unknow, A, B, Both };

/// <summary>
/// �÷���
/// 1.���������ʱ��Ҫ�ڹ��캯���д������������������nullptr����Ҫʹ��SetFather��������
/// 2.ʹ��ChangeFather���Դ�ԭ��������ɾ���Լ���Ȼ����Լ���ӽ��·�����
/// 3.ʹ��LeaveFather���Դ�ԭ��������ɾ���Լ�
/// </summary>
class VMObject
{
public:
	friend class SMObject;
	//���캯�������ԣ�VM�ţ�������������
	VMObject(VirtualMachine _VMProperty, int _vm_id, SMObject* _father) :
		VMProperty(_VMProperty), VM_ID(_vm_id) 
	{
		if (_father)
		{
			_father->AddChild(this);
		}
		father = _father;
	}

	~VMObject()
	{
		if (father) father->RemoveChild(this);
	}

	//��ʼ��father��������father��᷵��false
	bool SetFather(SMObject* _father)
	{
		if (father) return false;			//�Ѿ��и�����
		return _father->AddChild(this);
	}

	//�޸ļĴ�ķ����������ԭ��������ɾ���Լ��������ԭ�������Ļ�����
	bool ChangeFather(SMObject* _newfather)
	{
		if (father) father->RemoveChild(this);
		return _newfather->AddChild(this);
	}

	//��ԭ���������뿪
	bool LeaveFather()
	{
		if (!father) return false;			//�������ǹ¶�
		return father->RemoveChild(this);
	}
	
	VM_NodeType GetNodeType() const { return nodeType; }	//��ȡ�ڵ�����
	void SetNodeType(VM_NodeType _nodeType) { nodeType = _nodeType; }
	VirtualMachine GetProperty() const { return VMProperty; }	//��ȡ����
	int GetID() const { return VM_ID; }						//��ȡVM_ID
	
private:

	VirtualMachine VMProperty;		//����
	int VM_ID;						//�����ID
	VM_NodeType nodeType;			//������ڵ�����
	SMObject* father;				//���Ĵ�ķ�����
	
};
