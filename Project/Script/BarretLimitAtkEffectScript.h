#pragma once

#include <Engine\Script.h>

namespace ff7r
{
	class BarretLimitAtkEffectScript : public Script
	{
	public:
		BarretLimitAtkEffectScript();
		~BarretLimitAtkEffectScript();

		CLONE(BarretLimitAtkEffectScript);

		virtual void Awake() override;
		virtual void Tick() override;

		virtual void OnEnable() override;
	private:

	};
}
