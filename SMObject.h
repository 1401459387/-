#pragma once
#include "VMObject.h"
#include <algorithm>
#include "ServerMachine.h"
#include <unordered_map>

class VMObject;
class ServerMachine;

//��������ǰĳ�ڵ�״̬
struct ServerState
{
	int core;
	int memory;
	ServerState() : core(0), memory(0) {}
	ServerState(int _core, int _memory) : core(_core), memory(_memory) {}
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
private:
	const ServerMachine SMProperty;
	ServerState nodeA;
	ServerState nodeB;
	int SM_Id;
	int true_id;
	unordered_map<int, VMObject *> childs;

public:
	friend class VMObject;

	SMObject(const ServerMachine& _SMProperty,int _sm_id);

	//������Ҫ������������Ϊ���������ᱻ����
	//~SMObject() {}

	//���˷�������ǰ�Ƿ��Ż���û���������ʱ���ǲ���Ҫÿ�շ��õģ�
	bool IsRunning() const;

	int GetDailyCost() const; //��ȡÿ�����ķ���

	//���������������������fatherΪ�Լ������������и�������ԭ���׻�ɾ����
	bool AddChild(VMObject* child);
	//�ж��ܷ��Ŀ�����������Ϊ�Լ���child
	bool AbleToAddChild(VMObject* child);
	
	//ɾ����������ᵼ���������fatherΪ�գ�
	bool RemoveChild(VMObject* child);
	
	//ɾ����������ᵼ���������fatherΪ�գ�
	bool RemoveChild(int vm_id);

	string Gettype() { return SMProperty.GetModelType(); }

	void SetId(int id) { this->SM_Id = id; }
	void SetTrueId(int id){ this->true_id = id; }
	int GetTrueId () { return true_id; }
	int GetId() const { return SM_Id; }
	int GetBMemo() const { return nodeB.memory; }
	int GetAMemo() const { return nodeA.memory; }
	int GetBCore() const { return nodeB.core; }
	int GetACore() const { return nodeA.core; }
	const ServerMachine& GetProperty() const { return SMProperty; }

	bool operator< (const SMObject& b)
	{
		MachineType type = SMProperty.GetMechType();
		if (type == MachineType::HighCore)
		{
			if (nodeA.core == b.nodeA.core) return nodeB.core < b.nodeB.core;
			return nodeA.core < b.nodeA.core;
		}
		else if (type == MachineType::HighMemory)
		{
			if (nodeA.memory == b.nodeA.memory) return nodeB.memory < b.nodeB.memory;
			return nodeA.memory < b.nodeA.memory;
		}
		else
		{
			int max1 = min(nodeA.core, nodeA.memory);
			int max2 = min(b.nodeA.core, b.nodeA.memory);
			if (max1 == max2)
			{
				return min(nodeB.core, nodeB.memory) < min(b.nodeB.core, b.nodeB.memory);
			}
			return max1 < max2;

		}
	}

	bool operator>(const SMObject& b)
	{
		MachineType type = SMProperty.GetMechType();
		if (type == MachineType::HighCore)
		{
			if (nodeA.core == b.nodeA.core) return nodeB.core > b.nodeB.core;
			return nodeA.core > b.nodeA.core;
		}
		else if (type == MachineType::HighMemory)
		{
			if (nodeA.memory == b.nodeA.memory) return nodeB.memory > b.nodeB.memory;
			return nodeA.memory > b.nodeA.memory;
		}
		else
		{
			int max1 = min(nodeA.core, nodeA.memory);
			int max2 = min(b.nodeA.core, b.nodeA.memory);
			if (max1 == max2)
			{
				return min(nodeB.core, nodeB.memory) > min(b.nodeB.core, b.nodeB.memory);
			}
			return max1 > max2;

		}
	}

	bool operator<=(const SMObject& b)
	{
		return !(*this > b);
	}

	bool operator>=(const SMObject& b)
	{
		return !(*this < b);
	}

	bool operator==(const SMObject& b)
	{
		return nodeA.core == b.nodeA.core && nodeB.core == b.nodeB.core;
	}

	bool operator!=(const SMObject& b)
	{
		return !(*this == b);
	}
};