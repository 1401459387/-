#pragma once
#include "VMObject.h"
#include <unordered_map>

//��������ǰĳ�ڵ�״̬
struct ServerState
{
	int core;
	int memory;
	ServerState() :core(0), memory(0) {}
	ServerState(int _core, int _memory):core(_core), memory(_memory) {}
};

/// <summary>
/// �÷���
/// 1.���캯�������������������ͺͷ�������
/// 2.ʹ��AddChild��������
/// 3.ʹ��RemoveChild�Ƴ���������ᵼ���������fatherΪ�գ�
/// 4.ʹ��IsRunning�жϷ�������ʱ�Ƿ���Ҫ֧��ÿ�շ���
/// </summary>
class SMObject
{	
public:
	friend class VMObject;

	SMObject(ServerMachine _SMProperty, int _sm_id) :SMProperty(_SMProperty), SM_ID(_sm_id)
	{
		int sumCore = SMProperty.GetCore();
		int sumMemory = SMProperty.GetMemoryCapacity();
		nodeA.core = sumCore >> 1;
		nodeA.memory = sumMemory >> 1;
		nodeB.core = nodeA.core;
		nodeB.memory = nodeA.memory;
	}

	//������Ҫ������������Ϊ���������ᱻ����
	//~SMObject() {}

	//���˷�������ǰ�Ƿ��Ż���û���������ʱ���ǲ���Ҫÿ�շ��õģ�
	bool IsRunning() const
	{
		return childs.empty();		//����ǿյ�˵��û�������������
	}

	int GetDailyCost() const
	{
		return SMProperty.GetDailyCost();
	}

	//���������������������fatherΪ�Լ������������и�������ԭ���׻�ɾ����
	bool AddChild(VMObject* child)
	{
		if (child == nullptr) throw exception("����������������ӿ�ָ��");
		VirtualMachine property = child->GetProperty();
		bool isTwoNode = property.IsTwoNode();
		int core = property.GetCore();
		int memory = property.GetMemoryCapacity();
		int vm_id = child->GetID();
		if (isTwoNode)
		{
			core >>= 1;
			memory >>= 1;
			if (nodeA.core < core || nodeA.memory < memory || nodeB.core < core || nodeB.memory < memory)
			{
				return false;
			}
			else
			{
				nodeA.core -= core;
				nodeB.core -= core;
				nodeA.memory -= memory;
				nodeB.memory -= memory;
				child->SetNodeType(Both);
				childs[vm_id] = child;
				if (child->father) child->father->RemoveChild(child);
				child->father = this;
			}
		}
		else
		{
			//TODO ���������ĸ��ڵ���
			//�����ȷ���A�ڵ���
			if (nodeA.core < core || nodeA.memory < memory)
			{
				if (nodeB.core < core || nodeB.memory < memory)
				{
					throw exception("�����������´������");		//��������
				}
				else
				{
					nodeB.core -= core;
					nodeB.memory -= memory;
					child->SetNodeType(B);
					childs[vm_id] = child;
					if (child->father) child->father->RemoveChild(child);
					child->father = this;
				}
			}
			else
			{
				nodeA.core -= core;
				nodeA.memory -= memory;
				child->SetNodeType(A);
				childs[vm_id] = child;
				if (child->father) child->father->RemoveChild(child);
				child->father = this;
			}
		}
		return true;
	}
	//ɾ����������ᵼ���������fatherΪ�գ�
	bool RemoveChild(VMObject* child)
	{
		int vm_id = child->GetID();
		if (childs.find(vm_id) == childs.end()) throw exception("�������в�û�д������");

		VM_NodeType nodeType = child->GetNodeType();
		VirtualMachine property = child->GetProperty();
		int core = property.GetCore();
		int memory = property.GetMemoryCapacity();
		switch (nodeType)
		{
		case A:
			nodeA.core += core;
			nodeA.memory += memory;
			break;
		case B:
			nodeB.core += core;
			nodeB.memory += memory;
			break;
		case Both:
			core >>= 1;
			memory >>= 1;
			nodeA.core += core;
			nodeA.memory += memory;
			nodeB.core += core;
			nodeB.memory += memory;
			break;
		default:
			throw exception("Ҫɾ����������Ľڵ�����δ֪");
			break;
		}
		childs.erase(vm_id);
		return true;
	}
	//ɾ����������ᵼ���������fatherΪ�գ�
	bool RemoveChild(int vm_id)
	{
		if (childs.find(vm_id) == childs.end()) throw exception("�������в�û�д������");
		VMObject* child = childs[vm_id];

		return RemoveChild(child);
	}

private:
	ServerMachine SMProperty;
	int SM_ID;
	ServerState nodeA;
	ServerState nodeB;

	unordered_map<int, VMObject*> childs;
};