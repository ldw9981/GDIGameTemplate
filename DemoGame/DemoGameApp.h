#pragma once
#include "../D2DEngine/WinGameApp.h"

class DemoGameApp : public WinGameApp
{
public:
	//  virtual �� ����ص� ������
	// override�� keyword�� ����ϹǷν� �����Լ� "������" ��°��� Ȯ�����Ѵ�.
	// �θ� Ŭ������ �Լ��� virtual �� �ƴϸ� ������ ����.
	virtual void Initialize() override;
};
