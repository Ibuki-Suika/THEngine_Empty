#ifndef THTWEEN_H
#define THTWEEN_H

#include <Common\THCommon.h>

namespace THEngine
{
	class GameObject;
	class Tweener;

	class Tween : public Object
	{
	protected:
		GameObject* object = nullptr;

		bool finished = false;
		bool looped = false;
		bool started = false;

	public:
		std::function<void()> onFinished = []() {};

	public:
		Tween();
		Tween(const Tween& tween);
		virtual ~Tween();

		virtual Object* Clone() override;

		virtual void Update();

		virtual void DoTween();

		virtual void OnStart();
		virtual void OnFinish();
		virtual void OnReset();

		virtual void Bind(GameObject* object);

		inline bool IsFinished() { return finished; }

		inline void SetLooped(bool looped) { this->looped = looped; }

	public:
		friend class TweenManager;
		friend class TweenSequence;
	};

	////////////////////////////////////////////////////
	class TweenUnit : public Tween
	{
	protected:
		Tweener* tweener = nullptr;
		int duration;

	public:
		TweenUnit();
		TweenUnit(const TweenUnit& tweenUnit);
		virtual ~TweenUnit();

		virtual Object* Clone() override;

		virtual void DoTween() override;
		virtual void OnReset() override;
	};

	///////////////////////////////////////////////////
	class TweenSequence : public Tween
	{
	protected:
		ArrayList<Tween*> tweenList;
		int currentIndex = 0;

	public:
		TweenSequence();
		TweenSequence(const TweenSequence& sequence);
		virtual ~TweenSequence();

		virtual Object* Clone() override;

		virtual void DoTween() override;
		virtual void OnStart() override;
		virtual void OnReset() override;

		virtual void Bind(GameObject* object) override;

		inline void AddTween(Tween* tween)
		{
			tween->object = this->object;
			tweenList.Add(tween);
		}
	};
}

#endif