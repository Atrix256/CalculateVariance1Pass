#include <random>
#include <stdio.h>
#include <vector>

float Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + b * t;
}

float Variance_2Pass(const std::vector<float>& data)
{
    // get the average value (mean)
    float average_value = {};
    for (size_t index = 0; index < data.size(); ++index)
        average_value = Lerp(average_value, data[index], 1.0f / float(index + 1));

    // get the average of the (value - average value)^2
    float average_valueMinusMeanSquared = {};
    for (size_t index = 0; index < data.size(); ++index)
    {
        float valueMinusMeanSquared = data[index] - average_value;
        valueMinusMeanSquared *= valueMinusMeanSquared;
        average_valueMinusMeanSquared = Lerp(average_valueMinusMeanSquared, valueMinusMeanSquared, 1.0f / float(index + 1));
    }
    return average_valueMinusMeanSquared;
}

float Variance_1Pass(const std::vector<float>& data)
{
    // get the average (value) and average (value*value)
    float average_value = {};
    float average_valueSquared = {};
    for (size_t index = 0; index < data.size(); ++index)
    {
        float value = data[index];
        average_value = Lerp(average_value, value, 1.0f / float(index + 1));
        average_valueSquared = Lerp(average_valueSquared, value * value, 1.0f / float(index + 1));
    }

    // variance is absolute value of average(value*value) - (average_value*average_value)
    return abs(average_valueSquared - (average_value * average_value));
}

float Average(const std::vector<float>& values)
{
    float ret = 0.0f;
    for (size_t index = 0; index < values.size(); ++index)
        ret = Lerp(ret, values[index], 1.0f / float(index + 1));
    return ret;
}

void TestValues(const std::vector<float>& values, const char* label)
{
    float variance1Pass = Variance_1Pass(values);
    float variance2Pass = Variance_2Pass(values);
    float diff = abs(variance1Pass - variance2Pass);

    printf("===== %s =====\n", label);
    printf("Calculating variance of %zu numbers...\n\n", values.size());
    printf("1 Pass = %f\n2 Pass = %f\nDifference = %f\n\n", variance1Pass, variance2Pass, diff);
    printf("mean = %f\n", Average(values));
    printf("stdDev = %f\n\n", sqrt(variance1Pass));
}

int main(int argc, char** argv)
{
    // Test uniform distribution
    {
        static const unsigned int c_numValues = 1000;

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<float> dist(-1000.0f, 1000.0f);

        std::vector<float> values(c_numValues);
        for (float& f : values)
            f = dist(rng);

        TestValues(values, "Uniform");
    }

    // Test Gaussian distribution
    {
        static const unsigned int c_numValues = 1000;

        std::random_device rd;
        std::mt19937 rng(rd());
        std::normal_distribution<float> dist(100.0f, 20.0f);

        std::vector<float> values(c_numValues);
        for (float& f : values)
            f = dist(rng);

        TestValues(values, "Gaussian");
    }

    return 0;
}
