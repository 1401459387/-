#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ServerMachine.h"
#include "VirtualMachine.h"
#include "SMObject.h"
#include "VMObject.h"
#include <windows.h>
#include <ctime>

using namespace std;

//���������ͱ�
unordered_map<string, ServerMachine> serverMachines;
//��������ͱ�
unordered_map<string, VirtualMachine> virtualMachines;
//��ǰ�����˵ķ�������
unordered_map<int, SMObject*> currentSM;
//��ǰ�����˵��������
unordered_map<int, VMObject*> currentVM;
//��ǰ���������
map<int, string> requestList;

//ÿ�չ����б�
map<string, int> dailyPurchase;
//ÿ��Ǩ���б�
vector<tuple<int, int, VM_NodeType> > dailyMigration;
//ÿ�մ����б�
vector<pair<int, VM_NodeType> > dailyCreation;

//��ǰ�ķ�����ID�ŵ����ֵ
int max_sm_id = 0;

int rd() //�������������
{
    //srand(time(0));
    srand(GetTickCount64());
    return rand() % serverMachines.size();
}

void InitInput(ifstream* ifs)
{
    int N; //����������
    *ifs >> N;
    ifs->get();
    string catcher;
    string _model; //�ͺ�
    int _core, _mCp, _hC, _dC;
    for (int i = 0; i < N; ++i)
    {
        getline(*ifs, catcher);

        int len = catcher.length();
        catcher = catcher.substr(1, len - 2);
        replace(catcher.begin(), catcher.end(), ',', ' ');
        stringstream stm(catcher);
        stm >> _model;
        stm >> _core >> _mCp >> _hC >> _dC;
        serverMachines[_model] = ServerMachine(_model, _core, _mCp, _hC, _dC);
    }

    *ifs >> N; //���������
    ifs->get();
    bool _tN;
    for (int i = 0; i < N; ++i)
    {
        getline(*ifs, catcher);
        int len = catcher.length();
        catcher = catcher.substr(1, len - 2);
        replace(catcher.begin(), catcher.end(), ',', ' ');
        stringstream stm(catcher);
        stm >> _model;
        stm >> _core >> _mCp >> _tN;
        virtualMachines[_model] = VirtualMachine(_model, _core, _mCp, _tN);
    }
}

//����Ϊ��ǰ�������������һ�������
ServerMachine& ChooseServer(const VirtualMachine& vm_property)
{
    int vm_core = vm_property.GetCore();
    int vm_memory = vm_property.GetMemoryCapacity();

    if (!vm_property.IsTwoNode())
    {
        vm_core <<= 1;
        vm_memory <<= 1;
    }

    while (true)
    {
        int _rand = rd();
        auto iter = serverMachines.begin();
        while (--_rand && iter != serverMachines.end()) ++iter;
        if (iter == serverMachines.end()) iter = serverMachines.begin();
        ServerMachine& serverMachine = iter->second;
        int sm_core = serverMachine.GetCore();
        int sm_memory = serverMachine.GetMemoryCapacity();
        if (vm_core <= sm_core && vm_memory <= sm_memory) return serverMachine;
    }
    /*auto iter = serverMachines.begin();
    for (; iter != serverMachines.end(); ++iter)
    {
        ServerMachine& serverMachine = iter->second;
        int sm_core = serverMachine.GetCore();
        int sm_memory = serverMachine.GetMemoryCapacity();
        if (vm_core <= sm_core && vm_memory <= sm_memory) return serverMachine;
    }
    if (iter == serverMachines.end()) throw new exception("No server match!");*/
}

char GetStringFromNodeType(const VM_NodeType& nodeType)
{
    switch (nodeType)
    {
    case VM_NodeType::A:
        return 'A';
    case VM_NodeType::B:
        return 'B';
    default:
        return ' ';
    }
}

void DailyOutput()
{
    int Q = dailyPurchase.size();
    printf("(purchase,%d)\n", Q);
    for (auto iter = dailyPurchase.begin(); iter != dailyPurchase.end(); ++iter)
    {
        printf("(%s,%d)\n", iter->first.c_str(), iter->second);
    }
    int W = dailyMigration.size();
    printf("(migration,%d)\n", W);
    for (auto iter = dailyMigration.begin(); iter != dailyMigration.end(); ++iter)
    {
        int vm_id = get<0>(*iter);
        int sm_id = get<1>(*iter);
        VM_NodeType& nodeType = get<2>(*iter);
        if (nodeType == VM_NodeType::Both)
        {
            printf("(%d,%d)\n", vm_id, sm_id);
        }
        else
        {
            char s_nodeType = GetStringFromNodeType(nodeType);
            printf("(%d,%d,%c)\n", vm_id, sm_id, s_nodeType);
        }
    }

    for (auto iter = dailyCreation.begin(); iter != dailyCreation.end(); ++iter)
    {
        int sm_id = iter->first;
        VM_NodeType& nodeType = iter->second;
        if (nodeType == VM_NodeType::Both)
        {
            printf("(%d)\n",sm_id);
        }
        else
        {
            char s_nodeType = GetStringFromNodeType(nodeType);
            printf("(%d,%c)\n", sm_id, s_nodeType);
        }
    }
}

void DailyClear()
{
    requestList.clear();
    dailyPurchase.clear();
    dailyMigration.clear();
    dailyCreation.clear();
}

int main()
{
    clock_t start = clock();

    ifstream* ifs = new ifstream("training-1.txt");
    InitInput(ifs);

    int T;              //����
    int R;              //ÿ���������

    string catcher;
    string _model;
    string _opType;
    int _VM_id;

    *ifs >> T;
    while (T--)
    {
        *ifs >> R;
        ifs->get();
        //��ȡ���յĴ�������
        for (int i = 0; i < R; ++i)
        {
            getline(*ifs, catcher);
            int len = catcher.length();
            catcher = catcher.substr(1, len - 2);
            replace(catcher.begin(), catcher.end(), ',', ' ');
            stringstream stm(catcher);
            stm >> _opType;
            if (_opType == "add") 
            {
                stm >> _model >> _VM_id;
                //����һ̨�����
                requestList[_VM_id] = _model;
            }
            else //del   
            {
                stm >> _VM_id;
                //ɾ��һ̨�����
                if (requestList.find(_VM_id) != requestList.end())
                {
                    requestList.erase(_VM_id);
                }
                else
                {
                    //�Ǵ���ǰ������ɾ��
                    if (currentVM.find(_VM_id) != currentVM.end())
                    {
                        currentVM[_VM_id]->LeaveFather();

                        currentVM.erase(_VM_id);
                    }
                }
            }
        }
        //TODO ����Ǩ��
        //Ϊ�������Ѱ��Ŀ�������
        for (auto iter = requestList.begin(); iter != requestList.end(); ++iter)
        {
            VirtualMachine& vm_property = virtualMachines[iter->second];
            VMObject* vmObject = new VMObject(vm_property, iter->first, nullptr);
            currentVM[iter->first] = vmObject;

            auto iter2 = currentSM.begin();
            for (; iter2 != currentSM.end(); ++iter2)
            {
                if (iter2->second->AddChild(vmObject))
                {
                    //�ҵ��˿��������µķ�����
                    dailyCreation.push_back(make_pair(iter2->first, vmObject->GetNodeType()));
                    break;
                }
            }
            //�Ҳ������������µķ�����
            if (iter2 == currentSM.end())
            {
                ServerMachine& sm_property = ChooseServer(vm_property);
                SMObject* smObject = new SMObject(sm_property, max_sm_id);
                smObject->AddChild(vmObject);
                dailyCreation.push_back(make_pair(max_sm_id, vmObject->GetNodeType()));

                currentSM[max_sm_id] = smObject;
                if (dailyPurchase.find(sm_property.GetModelType()) != dailyPurchase.end())
                {
                    ++dailyPurchase[sm_property.GetModelType()];
                }
                else
                {
                    dailyPurchase[sm_property.GetModelType()] = 1;
                }

                ++max_sm_id;
            }
        }

        //���
        DailyOutput();

        //��������
        DailyClear();
    }

    //clock_t finish = clock();
    //cout << (finish - start) / 1000 << "s" << endl;
}