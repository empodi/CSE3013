#include "LinkedList.h"

//1. ���ø� Ŭ������ Ȯ���ؾ���
//2. Stack�������� Delete �Լ� �������ؾ���
//����: first, current_size�� class�� ��� �����̱� ������ this �����͸� ����Ͽ� �����;���
template <typename T>
//LinkedList class�� ��ӹ���
class Stack : public LinkedList<T> {
public:
	bool Delete(T& element) {
		//first�� 0�̸� false��ȯ
		if (this->first == 0)
			return false;
		// LinkedList�� �޸� Stack�� current�� ����Ű�� ���� ����
		Node<T>* current = this->first;
		element = current->data;

		this->first = current->link;
		delete current;

		(this->current_size)--;

		return true;
	}
};
