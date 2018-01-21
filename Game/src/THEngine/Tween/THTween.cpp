#include "THTween.h"
#include "THTweener.h"

namespace THEngine
{
	Tween::Tween()
	{
	}

	Tween::Tween(const Tween& tween) : Object(tween)
	{
		object = tween.object;
		onFinished = tween.onFinished;
		looped = tween.looped;
	}

	Tween::~Tween()
	{
	}

	Ptr<Object> Tween::Clone() const
	{
		return Ptr<Tween>::New(*this).Get();
	}

	void Tween::Update()
	{
		if (started == false)
		{
			OnStart();
		}

		if (finished == false)
		{
			DoTween();
		}
	}

	void Tween::DoTween()
	{
	}

	void Tween::OnReset()
	{
		started = false;
		finished = false;
	}

	void Tween::OnStart()
	{
		started = true;
	}

	void Tween::OnFinish()
	{
		finished = true;
		onFinished();
	}

	void Tween::Bind(Ptr<GameObject> object)
	{
		this->object = object.Get();
	}

	////////////////////////////////////////
	TweenUnit::TweenUnit()
	{
	}

	TweenUnit::TweenUnit(const TweenUnit& tweenUnit) : Tween(tweenUnit)
	{
	}

	TweenUnit::~TweenUnit()
	{
	}

	Ptr<Object> TweenUnit::Clone() const
	{
		return Ptr<TweenUnit>::New(*this).Get();
	}

	void TweenUnit::DoTween()
	{
		if (tweener != nullptr)
		{
			tweener->Update();
			if (tweener->elapsed >= tweener->duration)
			{
				if (looped)
				{
					OnReset();
				}
				else
				{
					OnFinish();
				}
			}
		}
	}

	void TweenUnit::OnReset()
	{
		Tween::OnReset();
	}

	////////////////////////////////////////
	TweenSequence::TweenSequence()
	{
		tweenList.SetBaseSize(5);
	}

	TweenSequence::TweenSequence(const TweenSequence& sequence) : Tween(sequence)
	{
		auto iter = const_cast<TweenSequence&>(sequence).tweenList.GetIterator();
		while (iter->HasNext())
		{
			auto tween = iter->Next();
			AddTween((Tween*)tween->Clone().Get());
		}
	}

	TweenSequence::~TweenSequence()
	{
	}

	Ptr<Object> TweenSequence::Clone() const
	{
		return Ptr<TweenSequence>::New(*this).Get();
	}

	void TweenSequence::DoTween()
	{
		auto tween = tweenList.Get(currentIndex);

		tween->Update();

		if (tween->IsFinished())
		{
			if (currentIndex + 1 < tweenList.Size())
			{
				currentIndex++;
			}
			else
			{
				if (looped)
				{
					OnReset();
				}
				else
				{
					OnFinish();
				}
			}
		}
	}

	void TweenSequence::OnStart()
	{
		Tween::OnStart();

		if (tweenList.Size() == 0)
		{
			OnFinish();
			return;
		}
	}

	void TweenSequence::OnReset()
	{
		currentIndex = 0;

		auto iter = tweenList.GetIterator();
		while (iter->HasNext())
		{
			iter->Next()->OnReset();
		}
	}

	void TweenSequence::Bind(Ptr<GameObject> object)
	{
		Tween::Bind(object);

		auto iter = tweenList.GetIterator();
		while (iter->HasNext())
		{
			iter->Next()->Bind(object);
		}
	}
}