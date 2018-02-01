#include "THMenuItem.h"
#include <Tween\THConcreteTween.h>

namespace THEngine
{
	MenuItem::MenuItem()
	{
		SetColor(colorUnselected);
		SetPixelAlign(true);
	}

	MenuItem::~MenuItem()
	{
	}

	void MenuItem::Update()
	{
		Sprite::Update();

		if (isTwinkling)
		{
			twinkleCounter++;
			if (twinkleCounter >= twinkleTime)
			{
				twinkleCounter = 0;
				isTwinkling = false;

				if (selected && shineEnabled)
				{
					Ptr<TweenSequence> sequence = Ptr<TweenSequence>::New();
					sequence->AddTween(Ptr<ColorTo>::New(shineColor, shineTime, Tweener::EASE_OUT).Get());
					sequence->AddTween(Ptr<ColorTo>::New(Vector3f(1.0f, 1.0f, 1.0f), shineTime, Tweener::EASE_OUT).Get());
					sequence->SetLooped(true);

					tweenShine = sequence.Get();
					AddTween(tweenShine);
				}

				return;
			}
			if (twinkleCounter % (2 * twinkleInterval) == 0)
			{
				SetColor(Vector3f(1.0f, 1.0f, 1.0f));
			}
			else if (twinkleCounter % (2 * twinkleInterval) == twinkleInterval)
			{
				SetColor(Vector3f(0.25f, 0.25f, 0.25f));
			}
		}
	}

	void MenuItem::OnSelect()
	{
		selected = true;

		if (shineEnabled)
		{
			Ptr<TweenSequence> sequence = Ptr<TweenSequence>::New();
			sequence->AddTween(Ptr<ColorTo>::New(Vector3f(1.0f, 1.0f, 1.0f), shineTime, Tweener::EASE_OUT).Get());
			sequence->AddTween(Ptr<ColorTo>::New(shineColor, shineTime, Tweener::EASE_OUT).Get());
			sequence->SetLooped(true);

			tweenShine = sequence.Get();
			AddTween(tweenShine);
		}

		if (vibrateEnabled)
		{
			float x1 = Math::Random(0, xVibrateRange);
			float x2 = Math::Random(0, xVibrateRange);
			float y1 = Math::Random(0, yVibrateRange);
			float y2 = Math::Random(0, yVibrateRange);

			Ptr<TweenSequence> sequence = Ptr<TweenSequence>::New();
			sequence->AddTween(Ptr<MoveBy>::New(Vector3f(-x1, -y1, 0.0f), vibrateTime, Tweener::SIMPLE).Get());
			sequence->AddTween(Ptr<MoveBy>::New(Vector3f(x1 + x2, y1 + y2, 0.0f), vibrateTime, Tweener::SIMPLE).Get());
			sequence->AddTween(Ptr<MoveBy>::New(Vector3f(-x2, -y2, 0.0f), vibrateTime, Tweener::SIMPLE).Get());

			tweenVibrate = sequence.Get();
			AddTween(tweenVibrate);
		}
	}

	void MenuItem::OnClick()
	{
		if (twinkleEnabled)
		{
			isTwinkling = true;
		}

		if (tweenShine)
		{
			KillTween(tweenShine);
			tweenShine = nullptr;
		}
	}

	void MenuItem::OnDeselect()
	{
		selected = false;

		if (tweenShine)
		{
			KillTween(tweenShine);
			tweenShine = nullptr;
		}

		SetColor(colorUnselected);
	}

	void MenuItem::SetSelectable(bool selectable)
	{
		this->selectable = selectable;
		SetColor(colorUnselectable);
	}
}