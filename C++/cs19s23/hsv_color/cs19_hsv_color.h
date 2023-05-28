#ifndef CS19_HSV_COLOR_H_
#define CS19_HSV_COLOR_H_

#include <string>

namespace cs19 {
    /**
     * Represents an immutable HSV (hue, saturation, value) color where each of the three components is 
     * represented as a float. Offers a few overloaded operators and other functions, including several 
     * related to converting a HSV color to its equivalent 24-bit RGB components.
    */
    class HsvColor {
        public:
            HsvColor();

            /**
             * Constructs a new HsvColor with the given HSV components.
             * @param hue the given hue of this color
             * @param saturation the given saturation of this color
             * @param value the given value of this color
             * @return a new HsvColor object
            */
            HsvColor(float hue, float saturation, float value);

            //HsvColor(HsvColor &old);

            /**
             * Returns a complementary color, with the hue shifted 180°.
             * @return the complementary shifted color
            */
            HsvColor operator~() const;

            /**
             * Returns a color that is linearly interpolated between this and another
             * (i.e., with the H/S/V components averaged between the two colors).
             * @param HsvColor self HsvColor object
             * @param that another HsvColor object
             * @return the interpolated color
            */
           HsvColor operator| (const HsvColor &that) const;

            /**
             * Returns the grayscale (desaturated) version of this color.
             * @return the grayscale version of this color
            */
            HsvColor grayscale() const;

            /**
             * Returns a CSS-compatible color string in 24-bit RGB hexadecimal format
             * @return the RGB hexadecimal format color
            */
            std::string to_hex_string() const;

            /**
             * Returns a CSS-compatible color string in 24-bit RGB decimal format
             * @return the RGB decimal format color
            */
            std::string to_rgb_string() const;

            /**
             * Returns the hue component of this color in degrees
             * @return the degree hue component
            */
            float hue() const;

            /**
             * Returns the saturation component of this color as a percentage
             * @return the percentage saturation component
            */
            float saturation() const;

            /**
             * Returns the value component of this color as a percentage
             * @return the percentage value component
            */
            float value() const;

            /**
             * Returns the closest 8-bit red component of this color in RGB
             * @return the RGB red component
            */
            int red() const;

            /**
             * Returns the closest 8-bit green component of this color in RGB
             * @return the RGB green component
            */
            int green() const;

            /**
             * 	Returns the closest 8-bit blue component of this color in RGB
             * @return the RGB blue component
            */
            int blue() const;

        private:
            float _hue {};
            float _saturation {};
            float _value {};
            float C {};
            float X {};
            float m {};
            float r {};
            float g {};
            float b {};
    };
}

#endif // CS19_HSV_COLOR_H