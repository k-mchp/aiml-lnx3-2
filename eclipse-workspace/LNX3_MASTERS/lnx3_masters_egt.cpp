#include <egt/ui>           // Includes the EGT UI library

#include <string>           // Includes the C++ standard string class

#include <memory>           // Includes smart pointers like std::shared_ptr
#include <egt/uiloader.h>   // Load the UI Loader from EGT
#include <iostream>         // Includes the standard I/O stream objects
#include <fstream>          // Includes file stream classes
#include <stdio.h>          // C standard input/output libraries
#include <stdlib.h>         // C standard general utilities library
#include <unistd.h>         // For POSIX operating system API (e.g., sleep, close)
#include <egt/animation.h>  // Includes EGT animation support

#include <sys/ioctl.h>      // For ioctl system call

#include <chrono>           // Includes facilities for time utilities

#include <egt/text.h>       // Includes EGT text handling components

#include <cstdio>           // For sprintf, printf

#include <fcntl.h>          // For file control options (open, O_RDWR, etc.)

#include <linux/i2c-dev.h>  // For I2C device control

#define DEV_GPIO  "/dev/gpiochip0"  // Defines the device file for GPIO
#include <linux/i2c.h>              // For I2C driver interface
#include <linux/i2c-dev.h>          // For I2C device control
#include <linux/gpio.h>             // For GPIO control
#define DEV_I2C "/dev/i2c-1"        // Defines the I2C device file
#define SLAVE_ADDR 0x4F             // Slave address of Temp Sensor in I/O1 Xplained Pro
#define REG_TEMP 0x00               // Temperature register address
#define REG_CONFIG_MSB 0x01         // Configuration register MSB address
#define REG_CONFIG_LSB 0x02         // Configuration register LSB address

using namespace std;
// Use the standard namespace
using namespace egt;
// Use the EGT namespace

// Declaration of the Temperature Data Read Function
extern "C" {
float outfunction(); // Declare the C-style function for reading temperature data
}

int fd;                             // File descriptor for GPIO device
struct gpiohandle_request req;      // GPIO handle request structure

std::shared_ptr<PeriodicTimer> timer; // Shared pointer to a periodic timer object

// Function to setup Slider step increment and value

void setup_Slider_Value(egt::Slider &slider,
		std::shared_ptr<egt::v1::Label> &label) {

//  TODO: Modify the step increment value to 200

	const int step_increment = 100;

// Connect value change event of slider to update label and slider value
	slider.on_value_changed([&label, &slider, &step_increment]() {
// TODO: Modify the code line for Updating the label with the slider value
// Hint: Concatenate the slider.value to the existing “$” String
// Example: If the slider value is 200, the label should display "$ 200"
		label->text("$ ");

		int value = slider.value();

// Adjust the value to be a multiple of step_increment within the range of 0 to 1000
		value = std::max(0, std::min(1000, (value / step_increment) * step_increment));

// Update the slider's value
		slider.value(value);

	});
}

// Function to update the temperature Data in the label

void update_Label_with_Timer(
		std::shared_ptr<egt::Label> &label_Temp_Value_screen1,
		std::shared_ptr<egt::Label> &label_Temp_Value_screen2,
		std::shared_ptr<egt::Label> &label_Temp_Value_screen3,
		std::shared_ptr<egt::Label> &label_Temp_Value_screen4,
		std::shared_ptr<egt::Label> &label_Temp_Value_screen5,
		std::shared_ptr<egt::Label> &label_Temp_Value_screen6) {
	label_Temp_Value_screen1->color(egt::Palette::ColorId::label_text,
			egt::Color(0xFFFFFF));
	label_Temp_Value_screen1->font(egt::Font(20));
	label_Temp_Value_screen1->align(
			egt::AlignFlag::top | egt::AlignFlag::right);
	label_Temp_Value_screen1->margin(10);

	label_Temp_Value_screen2->color(egt::Palette::ColorId::label_text,
			egt::Color(0xFFFFFF));
	label_Temp_Value_screen2->font(egt::Font(20));
	label_Temp_Value_screen2->align(
			egt::AlignFlag::top | egt::AlignFlag::right);
	label_Temp_Value_screen2->margin(10);

	label_Temp_Value_screen3->color(egt::Palette::ColorId::label_text,
			egt::Color(0xFFFFFF));
	label_Temp_Value_screen3->font(egt::Font(20));
	label_Temp_Value_screen3->align(
			egt::AlignFlag::top | egt::AlignFlag::right);
	label_Temp_Value_screen3->margin(10);

	label_Temp_Value_screen4->color(egt::Palette::ColorId::label_text,
			egt::Color(0xFFFFFF));
	label_Temp_Value_screen4->font(egt::Font(20));
	label_Temp_Value_screen4->align(
			egt::AlignFlag::top | egt::AlignFlag::right);
	label_Temp_Value_screen4->margin(10);

	label_Temp_Value_screen5->color(egt::Palette::ColorId::label_text,
			egt::Color(0xFFFFFF));
	label_Temp_Value_screen5->font(egt::Font(20));
	label_Temp_Value_screen5->align(
			egt::AlignFlag::top | egt::AlignFlag::right);
	label_Temp_Value_screen5->margin(10);

	label_Temp_Value_screen6->color(egt::Palette::ColorId::label_text,
			egt::Color(0xFFFFFF));
	label_Temp_Value_screen6->font(egt::Font(20));
	label_Temp_Value_screen6->align(
			egt::AlignFlag::top | egt::AlignFlag::right);
	label_Temp_Value_screen6->margin(10);

// Create periodic timer to update label
	timer = std::make_shared<PeriodicTimer>(1s);

#if 0
    // Disabled reference code block to update the temperature data from the sensor (I/O1 xplained Pro) as reference
    timer->on_timeout([&]() {
        auto y1 = outfunction();
        label_Temp_Value_screen1->text(egt::detail::format(y1, 1) + "°F");
        label_Temp_Value_screen2->text(egt::detail::format(y1, 1) + "°F");
        label_Temp_Value_screen3->text(egt::detail::format(y1, 1) + "°F");
        label_Temp_Value_screen4->text(egt::detail::format(y1, 1) + "°F");
        label_Temp_Value_screen5->text(egt::detail::format(y1, 1) + "°F");
        label_Temp_Value_screen6->text(egt::detail::format(y1, 1) + "°F");
    });
#else
	// Hardcoded temperature data block
	timer->on_timeout([&]() {
		const std::string temp_str = "75.0°F";
		label_Temp_Value_screen1->text(temp_str);
		label_Temp_Value_screen2->text(temp_str);
		label_Temp_Value_screen3->text(temp_str);
		label_Temp_Value_screen4->text(temp_str);
		label_Temp_Value_screen5->text(temp_str);
		label_Temp_Value_screen6->text(temp_str);
	});
#endif

	timer->start();
}

// Function to setup scrolling text

void setup_ScrollingText_animation(std::shared_ptr<egt::Window> &main_screen) {
	auto scrolling_text =
			std::make_shared<egt::Label>(
					"Elevating Your Banking Experience, Delivering Excellence with Every Transaction.");
	scrolling_text->color(egt::Palette::ColorId::label_text,
			egt::Color::rgb(0xFFFFFF));
	scrolling_text->resize(egt::Size(800, 50));
	scrolling_text->align(egt::AlignFlag::bottom);
	scrolling_text->text_align(egt::AlignFlag::center);
	egt::Font font("Noto Sans", 18, egt::Font::Weight::normal);
	scrolling_text->font(font);

	const int scroll_distance = 840;
	const std::chrono::milliseconds duration(5000);

// Easing function to return the values between start point and end point for progression of animation

	auto easing_func = [](double t) {
		return t * t;
	};

//  TODO: In the instantiation of the egt::AutoAnimation class:
//               - Replace the current start value scroll_distance  with 0.
//               - Replace the current end value 0 with scroll_distance.

	auto animation = std::make_shared<egt::AutoAnimation>(scroll_distance, 0,
			duration, easing_func, [scrolling_text](double value) {
// Update the x-position of the scrolling text widget based on the current animation value

				scrolling_text->x(static_cast<int>(value));
			});

// Start the animation
	animation->start();

// Timer to restart the animation
	auto scroll_timer = std::make_shared<egt::PeriodicTimer>(
			std::chrono::milliseconds(duration.count() + 1000));
	scroll_timer->on_timeout([animation]() {
		animation->start();
	});
	scroll_timer->start();

// Ensure scrolling_text, animation, and scroll_timer stay in scope
	static std::shared_ptr<egt::Label> global_scrolling_text = scrolling_text;
	static std::shared_ptr<egt::AutoAnimation> global_animation = animation;
	static std::shared_ptr<egt::PeriodicTimer> global_timer = scroll_timer;

	main_screen->add(scrolling_text);
}

/////////////////////////////////////////////////////////
///// This part of code is for GPIO status monitoring explanation of this code will be in Lab3

class GpioEventMonitor {
public:

	using Callback = std::function<void(gpioevent_data& event)>;

	GpioEventMonitor() = delete;

	GpioEventMonitor(int fd1, Callback callback) :
			input(egt::Application::instance().event().io()), callback(
					std::move(callback)), buffer(1) {
// assign fd1 to input stream
		input.assign(fd1);

// register first async read
		egt::asio::async_read(input, egt::asio::buffer(buffer),
				std::bind(&GpioEventMonitor::handle_read, this,
						std::placeholders::_1));
	}

	void handle_read(const egt::asio::error_code &error) {
		if (error) {
			std::cout << "handle_read: " << error.message() << std::endl;
			return;
		}

		callback(buffer.front());

// register any additional async read
		egt::asio::async_read(input, egt::asio::buffer(buffer),
				std::bind(&GpioEventMonitor::handle_read, this,
						std::placeholders::_1));
	}

private:
// input wrapper around the fd1
	egt::asio::posix::stream_descriptor input;
// registered callback
	Callback callback;
// buffer to hold the gpioevent_data
	std::vector<gpioevent_data> buffer;
};

//////////////////////////////////

int main(int argc, char **argv) {

	egt::Application app(argc, argv);
	egt::experimental::UiLoader loader;
	egt::TopWindow window1;

	auto window = std::static_pointer_cast<egt::Window>(
			loader.load("file:ui.xml"));

//Screens definition

	auto main_screen = window->find_child<egt::Window>("main_screen");

	auto Transaction_screen = window->find_child<egt::Window>(
			"Transaction_screen");

	auto keypad_screen = window->find_child<egt::Window>("keypad_screen");

	auto incorrect_pin_screen = window->find_child<egt::Window>(
			"incorrect_pin_screen");

	auto slider_screen = window->find_child<egt::Window>("slider_screen");

	auto Thank_you_screen = window->find_child<egt::Window>("Thank_you_screen");

//Label updation based on slider value

	auto label = window->find_child<egt::Label>("Withdrawal_Amount_Label");

// Variables to track input attempts and value of password being entered

	int attempt = 0;
	int input_val = 0;

// Variable to store the password input sequence from the keypad

	std::string input;

// Defining input text box
	auto text = window->find_child<egt::TextBox>("Input_Password_TextBox");

// Scrolling text using Animation function

	setup_ScrollingText_animation(main_screen);

//creation of labels for all the screens
	auto label_Temp_Value_screen1 = std::make_shared<egt::Label>("00°F");
	auto label_Temp_Value_screen2 = std::make_shared<egt::Label>("00°F");
	auto label_Temp_Value_screen3 = std::make_shared<egt::Label>("00°F");
	auto label_Temp_Value_screen4 = std::make_shared<egt::Label>("00°F");
	auto label_Temp_Value_screen5 = std::make_shared<egt::Label>("00°F");
	auto label_Temp_Value_screen6 = std::make_shared<egt::Label>("00°F");

	// Add label_Temp_Value_screen1 to the main_screen
	main_screen->add(label_Temp_Value_screen1);

	// Add label_Temp_Value_screen2 to the Transaction screen
	Transaction_screen->add(label_Temp_Value_screen2);

	// Add label_Temp_Value_screen3 to the keypad_screen
	keypad_screen->add(label_Temp_Value_screen3);

	// Add label_Temp_Value_screen4 to the incorrect_pin_screen
	incorrect_pin_screen->add(label_Temp_Value_screen4);

	// Add label_Temp_Value_screen5 to the slider_screen
	slider_screen->add(label_Temp_Value_screen5);

	// Add label_Temp_Value_screen6 to the thank_you_screen
	Thank_you_screen->add(label_Temp_Value_screen6);

//function call to update label with timer

	update_Label_with_Timer(label_Temp_Value_screen1, label_Temp_Value_screen2,
			label_Temp_Value_screen3, label_Temp_Value_screen4,
			label_Temp_Value_screen5, label_Temp_Value_screen6);

//Positioning of slider

	int screen_width = 800;
	int screen_height = 480;

	int slider_width = 387;
	int slider_height = 150;

	int slider_x = (screen_width - slider_width) / 2;
	int slider_y = (screen_height - slider_height) / 2;

//Creation of Slider

	egt::Slider slider(
			egt::Rect(slider_x, slider_y, slider_width, slider_height), 0, 1000,
			0);

//Slider Colour Selection

	slider.color(egt::Palette::ColorId::bg, egt::Color(0xFFFFFF)); // Cyan with a hint of sky blue
	slider.color(egt::Palette::ColorId::button_bg, egt::Color(0xFFFFFF)); // Cyan with a hint of sky blue
	slider.color(egt::Palette::ColorId::button_fg, egt::Color(0xFFFFFF)); // Cyan with a hint of sky blue

//Slider Properties

	slider.slider_flags().set( { egt::SliderF::SliderFlag::round_handle,
			egt::SliderF::SliderFlag::show_label });
	slider.live_update(true);

// function call to update slider on value changed

	setup_Slider_Value(slider, label);

// Add slider to slider_screen window

	slider_screen->add(slider);

///// This part of code is for GPIO status monitoring explanation of this code will be in Lab3

	auto popup_width = window1.width() / 2;
	auto popup_height = window1.height() / 2;

// Create the popup window with th1e calculated dimensions.
	egt::Popup popup(egt::Size(popup_width, popup_height));
	popup.color(egt::Palette::ColorId::bg, egt::Palette::white);

	auto pos_x = (window1.width() - popup_width) / 2;
	auto pos_y = (window1.height() - popup_height) / 2;

// Move the popup window to the calculated position.
	popup.move(egt::Point(pos_x, pos_y));
	egt::Label title(popup, "Intruder Alert!!");
	title.align(AlignFlag::expand_horizontal);
	Font new_font("Serif", 26);
	title.font(new_font);
	title.color(egt::Palette::ColorId::label_text, egt::Color(255, 255, 255));
	title.color(Palette::ColorId::label_bg, Palette::grey);
	title.fill_flags(Theme::FillFlag::solid);

	egt::Button button(popup, "Try again!!");
	button.align(AlignFlag::center);
	button.on_event([&](Event &event) {
		if (event.id() == EventId::pointer_click) {
			cout << "clicked!" << endl;
			popup.hide();
		}
	});

	int fd = open("/dev/gpiochip0", 0);
	if (fd < 0) {
		std::cerr << "Failed to open /dev/gpiochip0" << std::endl;
		return 1;
	}
	gpioevent_request ereq { };
	ereq.lineoffset = 109;  // GPIO pin 109 (PD13)
	ereq.handleflags = GPIOHANDLE_REQUEST_INPUT;
	ereq.eventflags = GPIOEVENT_REQUEST_BOTH_EDGES;
	if (ioctl(fd, GPIO_GET_LINEEVENT_IOCTL, &ereq) < 0)
		std::cerr << "GPIO_GET_LINEEVENT_IOCTL failed" << std::endl;

	gpiohandle_data data { };
	if (ioctl(ereq.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0)
		std::cerr << "GPIOHANDLE_GET_LINE_VALUES_IOCTL failed" << std::endl;

	GpioEventMonitor monitor(ereq.fd,
			[&main_screen, &window, &popup, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen](
					const gpioevent_data &event) {
				if (event.id == GPIOEVENT_EVENT_RISING_EDGE) // Check if GPIO status is LOGIC HIGH
				{

					popup.show_modal();
					main_screen->show();
					Transaction_screen->hide();
					keypad_screen->hide();
					slider_screen->hide();
					incorrect_pin_screen->hide();
					Thank_you_screen->hide();
				}

				window->on_event(
						[&main_screen, &window, &Transaction_screen, &popup,
								&keypad_screen, &slider_screen,
								&incorrect_pin_screen, &Thank_you_screen](
								egt::Event &event) {
							switch (event.key().keycode) {

							case egt::EKEY_3: {
								main_screen->show();

								popup.hide();
								Transaction_screen->hide();
								keypad_screen->hide();
								slider_screen->hide();
								incorrect_pin_screen->hide();
								Thank_you_screen->hide();
								break;

							}

							default:
								break;
							}
						}, { egt::EventId::keyboard_up });

			});

///////////////////////////////////////////////////////////////////////////////

// Lab-2 exercise 2: Try to observe how the external events from the gesture is handled

// window on_event to get back to main_screen when K4 is pressed

	window->on_event(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen](
					egt::Event &event) {
				switch (event.key().keycode) {

				case egt::EKEY_3: {
					main_screen->show();
					Transaction_screen->hide();
					keypad_screen->hide();
					slider_screen->hide();
					incorrect_pin_screen->hide();
					Thank_you_screen->hide();
					break;
				}
				case egt::EKEY_RIGHT: {
					// Logic to move to the second screen on a swipe right event

					main_screen->hide();
					Transaction_screen->show();
					keypad_screen->hide();
					slider_screen->hide();
					incorrect_pin_screen->hide();
					Thank_you_screen->hide();
					break;

				}

					// TODO: Try navigating to the main_screen from the second screen,

				case egt::EKEY_LEFT: {
					//   Enter your logic here for swipe left event
					break;
				}

				default:
					break;
				}
			}, { egt::EventId::keyboard_up });

// Define ImageButton & Buttons

	auto Enter_button = window->find_child<egt::ImageButton>("Enter_button");

	auto Withdrawal_button = window->find_child<egt::ImageButton>(
			"Withdrawal_button");

	auto retry_pin_button = window->find_child<egt::Button>("retry_pin_button");

	auto cancel_incorrect_pin_button = window->find_child<egt::Button>(
			"cancel_incorrect_pin_button");

	auto Get_cash_button = window->find_child<egt::ImageButton>(
			"Get_cash_button");

	auto cancel_thank_you_button = window->find_child<egt::ImageButton>(
			"cancel_thank_you_button");

//On_click Events

// Handles Enter_button click to hide all screens except for Transaction_screen.
	Enter_button->on_click(
			[&main_screen, &Transaction_screen, &keypad_screen, &slider_screen,
					&incorrect_pin_screen, &Thank_you_screen](egt::Event&) {
				main_screen->hide();
				Transaction_screen->show();
				keypad_screen->hide();
				slider_screen->hide();
				incorrect_pin_screen->hide();
				Thank_you_screen->hide();
			});

//Handles Withdrawal_button click to hide all screens except for keypad_screen

	Withdrawal_button->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen,
					&input, &input_val, &text](egt::Event&) {

// TODO: Write the actual withdrawal_button to hide all screens except for keypad_screen

// The below code ensures that when entering keypad screens, any existing input data in the text box is cleared and the value is set to zero.
				input.clear();
				input_val = 0;

			});

// Button to get back to the main_screen from the incorrect pin screen

	cancel_incorrect_pin_button->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen](
					egt::Event&) {
				main_screen->show();
				Transaction_screen->hide();
				keypad_screen->hide();
				slider_screen->hide();
				incorrect_pin_screen->hide();
				Thank_you_screen->hide();
			});

// Button to get back to the keypad screen from the incorrect pin screen

	retry_pin_button->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen](
					egt::Event&) {
				main_screen->hide();
				Transaction_screen->hide();
				keypad_screen->show();
				slider_screen->hide();
				incorrect_pin_screen->hide();
				Thank_you_screen->hide();
			});

// Button to proceed to the Thank you screen

	Get_cash_button->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen,
					&slider](egt::Event&) {

				if (slider.value() > 0) {
//					LED_OFF();
					main_screen->hide();
					Transaction_screen->hide();
					keypad_screen->hide();
					slider_screen->hide();
					incorrect_pin_screen->hide();
					Thank_you_screen->show();
				}
			});

//  Button to get back to the main screen from the Thank you screen for further Transactions

	cancel_thank_you_button->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen](
					egt::Event&) {
				main_screen->show();
				Transaction_screen->hide();
				keypad_screen->hide();
				slider_screen->hide();
				incorrect_pin_screen->hide();
				Thank_you_screen->hide();
			});

//keypad button definitions

	auto num_one = window->find_child<egt::ImageButton>("PIN_NUM_ONE");
	auto num_two = window->find_child<egt::ImageButton>("PIN_NUM_TWO");
	auto num_three = window->find_child<egt::ImageButton>("PIN_NUM_THREE");
	auto num_four = window->find_child<egt::ImageButton>("PIN_NUM_FOUR");

	auto num_five = window->find_child<egt::ImageButton>("PIN_NUM_FIVE");

	auto num_six = window->find_child<egt::ImageButton>("PIN_NUM_SIX");

	auto num_seven = window->find_child<egt::ImageButton>("PIN_NUM_SEVEN");

	auto num_eight = window->find_child<egt::ImageButton>("PIN_NUM_EIGHT");

	auto num_nine = window->find_child<egt::ImageButton>("PIN_NUM_NINE");

	auto num_zero = window->find_child<egt::ImageButton>("PIN_NUM_ZERO");

	auto num_clear = window->find_child<egt::ImageButton>("PIN_NUM_CLEAR");

	auto num_back = window->find_child<egt::ImageButton>("kEYPAD_BACK");

// Defining a vector to store pairs of keypad buttons and their corresponding numbers

	std::vector<std::pair<egt::ImageButton*, std::string>> numberButtons = { {
			num_one.get(), "1" }, { num_two.get(), "2" },
			{ num_three.get(), "3" }, { num_four.get(), "4" }, { num_five.get(),
					"5" }, { num_six.get(), "6" }, { num_seven.get(), "7" }, {
					num_eight.get(), "8" }, { num_nine.get(), "9" }, {
					num_zero.get(), "0" } };

// Defining a lambda function to handle button click events and manage input

	auto handleNumberButtonClick = [&](const std::string &number) {
		if (attempt < 1) {
			text->clear();
			attempt++;
		}

		text->append("* ");
		input += number;
		input_val++;

		if (input_val == 4) {
			input_val = 0;

			if (input == "1234") {
				text->clear();
				slider.value(0);
				input.clear();
				main_screen->hide();
				Transaction_screen->hide();
				keypad_screen->hide();
				slider_screen->show();
				incorrect_pin_screen->hide();
				Thank_you_screen->hide();
//				LED_ON();

			}

			else {
				text->clear();
				input.clear();
				main_screen->hide();
				Transaction_screen->hide();
				keypad_screen->hide();
				slider_screen->hide();
				incorrect_pin_screen->show();
				Thank_you_screen->hide();
//				LED_OFF();

			}
		}
	};

// Set up click handlers for each keypad button

	for (auto &pair : numberButtons) {
		auto button = pair.first;
		auto number = pair.second;
		button->on_click([&, number](egt::Event&) {
			handleNumberButtonClick(number);
		});
	}

// "Back" button to return to transaction screen from keypad screen

	num_back->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen,
					&text, &attempt, &input, &input_val](egt::Event&) {

				text->clear();
				main_screen->hide();
				Transaction_screen->show();
				keypad_screen->hide();
				slider_screen->hide();
				incorrect_pin_screen->hide();
				Thank_you_screen->hide();

			});

// "Clear" Button to Erase Entered Password in Text Box

	num_clear->on_click(
			[&main_screen, &window, &Transaction_screen, &keypad_screen,
					&slider_screen, &incorrect_pin_screen, &Thank_you_screen,
					&text, &attempt, &input, &input_val](egt::Event&) {

				text->clear();
				input_val = 0;
				input.clear();

			});

// Close the device node
	close(fd);

// Showing the top Window
	window1.show();
	return app.run();

}

// This part of code will have sample functions to Toggle LED and read temperature data from the I/O1 xplained Pro as reference

#if 0

// Function to initialize the GPIO for LED Toggle

int init_LED_GPIO() {
// Open gpio
	fd = open(DEV_GPIO, 0);
	if (fd < 0) {
		printf("ERROR: open %s ret=%d\n", DEV_GPIO, fd);
		return -1;
	}
//    struct gpiohandle_request req;
// Set gpio pin 62 output
	req.lineoffsets[0] = 62;
	req.lines = 1;
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	strcpy(req.consumer_label, "LED_PIN_62");
	int lhfd = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	if (lhfd < 0) {
		printf("ERROR get line handle lhdf=%d\n", lhfd);
		return -1;
	}

	return 0;
}

// Function to set GPIO pin 62 high for LED Toggle

void LED_OFF() {
	if (fd < 0) {
		printf("GPIO not initialized\n");
		return;
	}
	struct gpiohandle_data data;
	data.values[0] = 1;   // Set the value in the global gpiohandle_data struct
	int ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
	if (ret < 0) {
		printf("ERROR set line value ret=%d\n", ret);
	}
}

// Function to set GPIO pin 62 low for LED Toggle

void LED_ON() {
	if (fd < 0) {
		printf("GPIO not initialized\n");
		return;
	}

	struct gpiohandle_data data;
	data.values[0] = 0;  // Set the value in the global gpiohandle_data struct
	int ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
	if (ret < 0) {
		printf("ERROR set line value ret=%d\n", ret);
	}
}

//  Sample function to read data from a temperature sensor (I/O1 Xplained Pro - Temp sensor)



extern "C" float outfunction() {

	char tempstrbuf[80];
	string strtemp;
	float g_temperature;
	int fd;
	int ret;
	unsigned char buf[2];

// Open device node
	fd = open(DEV_I2C, O_RDWR);
	if (fd < 0) {
		printf("ERROR open %s ret=%d\n", DEV_I2C, fd);
		return -1;
	}

	if (ioctl(fd, I2C_SLAVE, SLAVE_ADDR) < 0) {
		printf("ERROR ioctl() set slave address\n");
		return -1;
	}

// Set resolution to 12-bit
	unsigned char config_buf[3] = { REG_CONFIG_MSB, 0x60, 0x00 }; // Configuration for 12-bit resolution
	if (write(fd, config_buf, 3) != 3) {
		perror("Error writing configuration registers");
		close(fd);
		return -1;
	}

// Read temperature
	buf[0] = REG_TEMP;
	ret = write(fd, &buf[0], 1);
	if (ret != 1) {
		printf("ERROR write() register address\n");
		return -1;
	}

// Read data
	buf[1] = 0;
	ret = read(fd, &buf[0], 2);
	if (ret != 2) {
		printf("ERROR read() data\n");
		return -1;
	}
// Convert temperature data to Celsius (12-bit resolution)
	g_temperature = ((buf[0] << 4) | (buf[1] >> 4)) * 0.0625;

// Round g_temperature to 1 decimal place
	g_temperature = round(g_temperature * 10) / 10.0;

// Convert Celsius to Fahrenheit
	float g_temperature_fahrenheit = (g_temperature * 9.0 / 5.0) + 32.0;

// Round g_temperature_fahrenheit to 1 decimal place
	g_temperature_fahrenheit = round(g_temperature_fahrenheit * 10) / 10.0;

// Print temperature in Fahrenheit with a single decimal point
	sprintf(tempstrbuf, "Temperature is %.1f°F\n", g_temperature_fahrenheit);

	strtemp = tempstrbuf;
	printf("%s", tempstrbuf);

// Close device node
	close(fd);

	return g_temperature_fahrenheit;
};

#endif

