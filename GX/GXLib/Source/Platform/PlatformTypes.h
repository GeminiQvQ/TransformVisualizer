#pragma once

namespace GX
{
	enum class Key
	{
		A, B, C, D, E, F, G, H, I, J,
		K, L, M, N, O, P, Q, R, S, T,
		U, V, W, X, Y, Z,
		N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Left, Right, Up, Down, Esc, Space, Backspace, Return,

		Invalid
	};

	enum class Modifier
	{
		Shift,
		Control,
		Alt,

		Invalid
	};

	enum class MouseButton
	{
		Left,
		Middle,
		Right,

		Invalid
	};
}