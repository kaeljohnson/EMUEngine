#pragma once

#include <stdio.h>
#include <queue>

#include "../Actions/ActionsEnum.h"


namespace Engine
{
	class EventHandlers
	{
		typedef void (*quit_handler_t)(std::queue<Actions>& refActionsQ);
	
		typedef void (*window_resize_event_handler_t)(std::queue<Actions>& refActionsQ);

		typedef void (*key_equals_down_event_handler_t)(std::queue<Actions>& refActionsQ);
		typedef void (*key_equals_up_event_handler_t)();

		typedef void (*key_esc_down_event_handler_t)(void);
		typedef void (*key_esc_up_event_handler_t)(void);

		typedef void (*key_zero_down_event_handler_t)(void);
		typedef void (*key_zero_up_event_handler_t)(void);

		typedef void (*key_one_down_event_handler_t)(void);
		typedef void (*key_one_up_event_handler_t)(void);

		typedef void (*key_two_down_event_handler_t)(void);
		typedef void (*key_two_up_event_handler_t)(void);

		typedef void (*key_three_down_event_handler_t)(void);
		typedef void (*key_three_up_event_handler_t)(void);

		typedef void (*key_four_down_event_handler_t)(void);
		typedef void (*key_four_up_event_handler_t)(void);

		typedef void (*key_five_down_event_handler_t)(void);
		typedef void (*key_five_up_event_handler_t)(void);

		typedef void (*key_six_down_event_handler_t)(void);
		typedef void (*key_six_up_event_handler_t)(void);

		typedef void (*key_seven_down_event_handler_t)(void);
		typedef void (*key_seven_up_event_handler_t)(void);

		typedef void (*key_eight_down_event_handler_t)(void);
		typedef void (*key_eight_up_event_handler_t)(void);

		typedef void (*key_nine_down_event_handler_t)(void);
		typedef void (*key_nine_up_event_handler_t)(void);


		typedef void (*key_q_down_event_handler_t)(void);
		typedef void (*key_q_up_event_handler_t)(void);

		typedef void (*key_w_down_event_handler_t)(void);
		typedef void (*key_w_up_event_handler_t)(void);

		typedef void (*key_e_down_event_handler_t)(void);
		typedef void (*key_e_up_event_handler_t)(void);

		typedef void (*key_r_down_event_handler_t)(void);
		typedef void (*key_r_up_event_handler_t)(void);

		typedef void (*key_t_down_event_handler_t)(void);
		typedef void (*key_t_up_event_handler_t)(void);

		typedef void (*key_y_down_event_handler_t)(void);
		typedef void (*key_y_up_event_handler_t)(void);

		typedef void (*key_u_down_event_handler_t)(void);
		typedef void (*key_u_up_event_handler_t)(void);

		typedef void (*key_i_down_event_handler_t)(void);
		typedef void (*key_i_up_event_handler_t)(void);

		typedef void (*key_o_down_event_handler_t)(void);
		typedef void (*key_o_up_event_handler_t)(void);

		typedef void (*key_p_down_event_handler_t)(void);
		typedef void (*key_p_up_event_handler_t)(void);

		typedef void (*key_a_down_event_handler_t)(void);
		typedef void (*key_a_up_event_handler_t)(void);

		typedef void (*key_s_down_event_handler_t)(void);
		typedef void (*key_s_up_event_handler_t)(void);

		typedef void (*key_d_down_event_handler_t)(void);
		typedef void (*key_d_up_event_handler_t)(void);

		typedef void (*key_f_down_event_handler_t)(void);
		typedef void (*key_f_up_event_handler_t)(void);

		typedef void (*key_g_down_event_handler_t)(void);
		typedef void (*key_g_up_event_handler_t)(void);

		typedef void (*key_h_down_event_handler_t)(void);
		typedef void (*key_h_up_event_handler_t)(void);

		typedef void (*key_j_down_event_handler_t)(void);
		typedef void (*key_j_up_event_handler_t)(void);

		typedef void (*key_k_down_event_handler_t)(void);
		typedef void (*key_k_up_event_handler_t)(void);

		typedef void (*key_l_down_event_handler_t)(void);
		typedef void (*key_l_up_event_handler_t)(void);

		typedef void (*key_z_down_event_handler_t)(void);
		typedef void (*key_z_up_event_handler_t)(void);

		typedef void (*key_x_down_event_handler_t)(void);
		typedef void (*key_x_up_event_handler_t)(void);

		typedef void (*key_c_down_event_handler_t)(void);
		typedef void (*key_c_up_event_handler_t)(void);

		typedef void (*key_v_down_event_handler_t)(void);
		typedef void (*key_v_up_event_handler_t)(void);

		typedef void (*key_b_down_event_handler_t)(void);
		typedef void (*key_b_up_event_handler_t)(void);

		typedef void (*key_n_down_event_handler_t)(void);
		typedef void (*key_n_up_event_handler_t)(void);

		typedef void (*key_m_down_event_handler_t)(void);
		typedef void (*key_m_up_event_handler_t)(void);

		typedef void (*left_arrow_key_down_event_handler_t)();
		typedef void (*left_arrow_key_up_event_handler_t)();

		typedef void (*up_arrow_key_down_event_handler_t)();
		typedef void (*up_arrow_key_up_event_handler_t)();

		typedef void (*right_arrow_key_down_event_handler_t)();
		typedef void (*right_arrow_key_up_event_handler_t)();

		typedef void (*down_arrow_key_down_event_handler_t)();
		typedef void (*down_arrow_key_up_event_handler_t)();

		typedef void (*key_tab_down_event_handler_t)(void);
		typedef void (*key_tab_up_event_handler_t)(void);

		typedef void (*key_capslock_down_event_handler_t)(void);
		typedef void (*key_capslock_up_event_handler_t)(void);

		typedef void (*key_lshift_down_event_handler_t)(void);
		typedef void (*key_lshift_up_event_handler_t)(void);

		typedef void (*key_enter_down_event_handler_t)(void);
		typedef void (*key_enter_up_event_handler_t)(void);

		typedef void (*key_space_bar_down_event_handler_t)(void);
		typedef void (*key_space_bar_up_event_handler_t)(void);

		typedef void (*mouse_button_left_down_event_handler_t)(void);
		typedef void (*mouse_button_left_up_event_handler_t)(void);

		typedef void (*mouse_button_middle_down_event_handler_t)(void);
		typedef void (*mouse_button_middle_up_event_handler_t)(void);

		typedef void (*mouse_button_right_down_event_handler_t)(void);
		typedef void (*mouse_button_right_up_event_handler_t)(void);

		typedef void (*mouse_button_move_event_handler_t)(int xPos, int yPos);
		typedef void (*mouse_scroll_event_handler_t)(int xDir, int yDir);


	public:
		quit_handler_t quit;
	
		window_resize_event_handler_t windowResize;

		key_equals_down_event_handler_t equalsDown;
		key_equals_up_event_handler_t equalsUp;

		// Number keys
		key_esc_down_event_handler_t escapeKeyDown;
		key_esc_up_event_handler_t escapeKeyUp;

		key_zero_down_event_handler_t zeroKeyDown;
		key_zero_up_event_handler_t zeroKeyUp;

		key_one_down_event_handler_t oneKeyDown;
		key_one_up_event_handler_t oneKeyUp;

		key_two_down_event_handler_t twoKeyDown;
		key_two_up_event_handler_t twoKeyUp;

		key_three_down_event_handler_t threeKeyDown;
		key_three_up_event_handler_t threeKeyUp;

		key_four_down_event_handler_t fourKeyDown;
		key_four_up_event_handler_t fourKeyUp;

		key_five_down_event_handler_t fiveKeyDown;
		key_five_up_event_handler_t fiveKeyUp;

		key_six_down_event_handler_t sixKeyDown;
		key_six_up_event_handler_t sixKeyUp;

		key_seven_down_event_handler_t sevenKeyDown;
		key_seven_up_event_handler_t sevenKeyUp;

		key_eight_down_event_handler_t eightKeyDown;
		key_eight_up_event_handler_t eightKeyUp;

		key_nine_down_event_handler_t nineKeyDown;
		key_nine_up_event_handler_t nineKeyUp;

		// Letter keys
		key_q_down_event_handler_t qKeyDown;
		key_q_up_event_handler_t qKeyUp;

		key_w_down_event_handler_t wKeyDown;
		key_w_up_event_handler_t wKeyUp;

		key_e_down_event_handler_t eKeyDown;
		key_e_up_event_handler_t eKeyUp;

		key_r_down_event_handler_t rKeyDown;
		key_r_up_event_handler_t rKeyUp;

		key_t_down_event_handler_t tKeyDown;
		key_t_up_event_handler_t tKeyUp;

		key_y_down_event_handler_t yKeyDown;
		key_y_up_event_handler_t yKeyUp;

		key_u_down_event_handler_t uKeyDown;
		key_u_up_event_handler_t uKeyUp;

		key_i_down_event_handler_t iKeyDown;
		key_i_up_event_handler_t iKeyUp;

		key_o_down_event_handler_t oKeyDown;
		key_o_up_event_handler_t oKeyUp;

		key_p_down_event_handler_t pKeyDown;
		key_p_up_event_handler_t pKeyUp;

		key_a_down_event_handler_t aKeyDown;
		key_a_up_event_handler_t aKeyUp;

		key_s_down_event_handler_t sKeyDown;
		key_s_up_event_handler_t sKeyUp;

		key_d_down_event_handler_t dKeyDown;
		key_d_up_event_handler_t dKeyUp;

		key_f_down_event_handler_t fKeyDown;
		key_f_up_event_handler_t fKeyUp;

		key_g_down_event_handler_t gKeyDown;
		key_g_up_event_handler_t  gKeyUp;

		key_h_down_event_handler_t hKeyDown;
		key_h_up_event_handler_t hKeyUp;

		key_j_down_event_handler_t jKeyDown;
		key_j_up_event_handler_t jKeyUp;

		key_k_down_event_handler_t kKeyDown;
		key_k_up_event_handler_t kKeyUp;

		key_l_down_event_handler_t lKeyDown;
		key_l_up_event_handler_t lKeyUp;

		key_z_down_event_handler_t zKeyDown;
		key_z_up_event_handler_t zKeyUp;

		key_x_down_event_handler_t xKeyDown;
		key_x_up_event_handler_t xKeyUp;

		key_c_down_event_handler_t cKeyDown;
		key_c_up_event_handler_t cKeyUp;

		key_v_down_event_handler_t vKeyDown;
		key_v_up_event_handler_t vKeyUp;

		key_b_down_event_handler_t bKeyDown;
		key_b_up_event_handler_t bKeyUp;

		key_n_down_event_handler_t nKeyDown;
		key_n_up_event_handler_t nKeyUp;

		key_m_down_event_handler_t mKeyDown;
		key_m_up_event_handler_t mKeyUp;

		left_arrow_key_down_event_handler_t leftArrowKeyDown;
		left_arrow_key_up_event_handler_t leftArrowKeyUp;

		up_arrow_key_down_event_handler_t upArrowKeyDown;
		up_arrow_key_up_event_handler_t upArrowKeyUp;

		down_arrow_key_down_event_handler_t downArrowKeyDown;
		down_arrow_key_up_event_handler_t downArrowKeyUp;

		right_arrow_key_down_event_handler_t rightArrowKeyDown;
		right_arrow_key_up_event_handler_t rightArrowKeyUp;

		// Misc keys.
		key_tab_down_event_handler_t tabKeyDown;
		key_tab_up_event_handler_t tabKeyUp;

		key_capslock_down_event_handler_t capslockKeyDown;
		key_capslock_up_event_handler_t capslockKeyUp;

		key_lshift_down_event_handler_t lShiftKeyDown;
		key_lshift_up_event_handler_t lShiftKeyUp;

		key_enter_down_event_handler_t enterKeyDown;
		key_enter_up_event_handler_t enterKeyUp;

		key_space_bar_down_event_handler_t spaceKeyDown;
		key_space_bar_up_event_handler_t spaceKeyUp;

		// Mouse.
		mouse_button_left_down_event_handler_t leftMouseButtonDown;
		mouse_button_left_up_event_handler_t leftMouseButtonUp;

		mouse_button_middle_down_event_handler_t middleMouseButtonDown;
		mouse_button_middle_up_event_handler_t middleMouseButtonUp;

		mouse_button_right_down_event_handler_t rightMouseButtonDown;
		mouse_button_right_up_event_handler_t rightMouseButtonUp;

		mouse_button_move_event_handler_t mouseMove;
		mouse_scroll_event_handler_t mouseScroll;

		EventHandlers();

		EventHandlers(const EventHandlers&) = delete;
		EventHandlers& operator=(const EventHandlers&) = delete;
		EventHandlers(EventHandlers&&) = delete;
		EventHandlers& operator=(EventHandlers&&) = delete;
	};
}