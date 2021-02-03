#ifndef TWOSF2WAV_SAMPLEDATA_H
#define TWOSF2WAV_SAMPLEDATA_H

#include <vector>
#include <cstdint>
class IInterpolator;

class SampleData : public std::vector<int32_t>
{
public:
  enum Format {
    Pcm8,
    Pcm16,
    Adpcm
  };

  SampleData();
  SampleData(uint32_t baseAddr, uint16_t loopStart, uint32_t loopLength, Format format);
  SampleData(const SampleData&) = default;
  SampleData(SampleData&&) = default;
  ~SampleData() = default;

  SampleData& operator=(const SampleData&) = default;
  SampleData& operator=(SampleData&&) = default;

  int32_t sampleAt(double time, IInterpolator* interp = nullptr) const;

  uint32_t baseAddr;
  uint16_t loopStart;
  uint32_t loopLength;

private:
  void loadPcm8();
  void loadPcm16();
  void loadAdpcm();
};

#endif
