#include "SMObject.h"

SMObject::SMObject(ServerMachine _SMProperty) : SMProperty(_SMProperty)
{
	int sumCore = SMProperty.GetCore();
	int sumMemory = SMProperty.GetMemoryCapacity();
	nodeA.core = sumCore >> 1;
	nodeA.memory = sumMemory >> 1;
	nodeB.core = nodeA.core;
	nodeB.memory = nodeA.memory;
}

bool SMObject::IsRunning() const
{
	return childs.empty(); //����ǿյ�˵��û�������������
}

int SMObject::GetDailyCost() const //��ȡÿ�����ķ���
{
	return SMProperty.GetDailyCost();
}

bool SMObject::AddChild(VMObject* child)
{
	if (child == nullptr)
		return false;//throw exception("����������������ӿ�ָ��");
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
			if (child->father)
				child->father->RemoveChild(child);
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
				return false; //throw exception("�����������´������"); //��������
			}
			else
			{
				nodeB.core -= core;
				nodeB.memory -= memory;
				child->SetNodeType(B);
				childs[vm_id] = child;
				if (child->father)
					child->father->RemoveChild(child);
				child->father = this;
			}
		}
		else
		{
			nodeA.core -= core;
			nodeA.memory -= memory;
			child->SetNodeType(A);
			childs[vm_id] = child;
			if (child->father)
				child->father->RemoveChild(child);
			child->father = this;
		}
	}
	return true;
}

bool SMObject::RemoveChild(VMObject* child)
{
	int vm_id = child->GetID();
	if (childs.find(vm_id) == childs.end())
		return false; //throw exception("�������в�û�д������");

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
		return false; //throw exception("Ҫɾ����������Ľڵ�����δ֪");
		break;
	}
	this->childs.erase(vm_id);
	return true;
}
bool SMObject::RemoveChild(int vm_id)
{
	if (this->childs.find(vm_id) == childs.end())
		return false; //throw exception("�������в�û�д������");
	VMObject* child = this->childs[vm_id];

	return RemoveChild(child);
}
