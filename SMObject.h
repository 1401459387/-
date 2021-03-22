#pragma once
#include "VMObject.h"
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
	const ServerMachine& SMProperty;
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
	
	//ɾ����������ᵼ���������fatherΪ�գ�
	bool RemoveChild(VMObject* child);
	
	//ɾ����������ᵼ���������fatherΪ�գ�
	bool RemoveChild(int vm_id);

	string Gettype() { return SMProperty.GetModelType(); }

	void SetId(int id) { this->SM_Id = id; }
	void SetTrueId(int id){ this->true_id = id; }
	int GetTrueId () { return true_id; }
	int GetId() const { return SM_Id; }
};