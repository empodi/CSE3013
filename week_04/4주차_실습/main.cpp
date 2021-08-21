#include <stdio.h>
#include "Stack.h"

void prnMenu() {
	cout << "*******************************************" << endl;
	cout << "* 1. ����    2. ����    3. ���   4. ���� *" << endl;
	cout << "*******************************************" << endl;
	cout << endl;
	cout << "���Ͻô� �޴��� ����ּ���: ";
}

int main() {
	int mode, selectNumber, tmpItem;
	LinkedList<int>* p;
	bool flag = false;

	std::cout << "�ڷᱸ�� ����(1: Stack, Other: Linked List): ";
	cin >> mode;

	if (mode == 1)
		p = new Stack<int>();    // ������ �����ϴ� ����
	else
		p = new LinkedList<int>();


	do {
		prnMenu();
		std::cin >> selectNumber;

		switch (selectNumber) {
		case 1:
			std::cout << "���Ͻô� ���� �Է����ּ���: ";
			std::cin >> tmpItem;    p->Insert(tmpItem);
			std::cout << tmpItem << "�� ���ԵǾ����ϴ�." << endl;
			break;

		case 2:
			if (p->Delete(tmpItem) == true)
				std::cout << tmpItem << "�� �����Ǿ����ϴ�." << endl;

			else std::cout << "����ֽ��ϴ�. ���� ����" << endl;
			break;

		case 3:
			std::cout << "ũ��: " << p->GetSize() << endl;
			p->Print();
			break;

		case 4:
			flag = true;     break;

		default:
			std::cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
			break;

		}

		if (flag) break;

	} while (1);
	
	return 0;
}

