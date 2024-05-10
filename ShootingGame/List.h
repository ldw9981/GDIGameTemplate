#pragma once
template<typename T>
class List
{
public:
	struct Node
	{
		T data;
		Node* pNext = nullptr;
		Node* pPrev = nullptr;
	};
	Node* m_pHead = nullptr;
	Node* m_pTail = nullptr;

	Node* push_back(T data)
	{
		Node* pNewNode = new Node;
		pNewNode->data = data;
		if (m_pTail == nullptr || m_pHead == nullptr)
		{
			m_pHead = pNewNode;
			m_pTail = pNewNode;
		}
		else
		{
			m_pTail->pNext = pNewNode;
			pNewNode->pPrev = m_pTail;
			m_pTail = pNewNode;
		}
		return pNewNode;
	}

	Node* erase(Node* pNode)
	{
		if (pNode->pPrev != nullptr)	// 앞노드 포인터 처리
		{
			pNode->pPrev->pNext = pNode->pNext;

		}
		if (pNode->pNext != nullptr)	// 뒷노드 포인터 처리
		{
			pNode->pNext->pPrev = pNode->pPrev;
		}

		// Head,Tail 변경
		if (pNode == m_pHead)
		{
			m_pHead = pNode->pNext;
		}
		if (pNode == m_pTail)
		{
			m_pTail = pNode->pPrev;
		}
		Node* pNodeNext = pNode->pNext;	// 다음노드 리턴
		delete pNode; // 해제
		return pNodeNext;
	}

	void clear()
	{
		Node* pCurr = m_pHead;
		while (pCurr != nullptr)
		{
			pCurr = erase(pCurr);
		}
	}
};

