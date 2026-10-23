/*
   SSGazeHelperProtocol.h: the single in-memory record the 64-bit gaze helper
   (SimonSaysGaze64.exe) shares with the 32-bit app (REQ-F98).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSGazeHelperProtocol_h
#define SSGazeHelperProtocol_h

#include <windows.h>

// Licence-relevant invariant (REQ-N08, Tobii EULA v3.0 §1.1 / §1.3): the record
// lives in an UNNAMED pagefile-backed section whose handle only the helper
// inherits, so no other process can find or open it. It holds one gaze sample,
// overwritten on every update, never written to disk, never sent anywhere.
//
// Every field is 4 bytes wide so the layout is identical in the 32-bit app and
// the 64-bit helper. Bump kSSGazeHelperVersion on any layout change.

constexpr DWORD kSSGazeHelperMagic = 0x5A475353; // 'SSGZ'
constexpr DWORD kSSGazeHelperVersion = 1;

struct SSGazeHelperRecord
{
  DWORD         magic;            // kSSGazeHelperMagic once the helper initialised it
  DWORD         version;          // kSSGazeHelperVersion
  volatile LONG seq;              // seqlock: odd while the helper is writing
  DWORD         heartbeatTick;    // GetTickCount() of the helper's last pump

  // Status (REQ-F101: safe to report — none of it comes from the sensors).
  LONG          engineUsable;     // 1 when the helper loaded a usable 64-bit engine
  LONG          engineMajor, engineMinor, engineRevision, engineBuild;
  LONG          lastDeviceCreateError; // tobii_error_t of the last create; 0 = ok

  // Live state (REQ-F94 / REQ-F29) — in memory only, never reported in dumps.
  LONG          deviceOpen;       // 1 while created + subscribed
  LONG          anyCallback;      // lastCallbackTick is meaningful
  DWORD         lastCallbackTick; // GetTickCount() of the last callback, any validity

  // The one gaze sample (REQ-N08): normalized coordinates, overwritten each time.
  LONG          sampleValid;
  DWORD         sampleTick;
  float         normX;
  float         normY;
};
static_assert( sizeof( SSGazeHelperRecord ) == 17 * 4, "record layout must match across 32/64-bit" );

// Writer side (helper). Single writer, so a plain increment pair suffices.
inline void SSGazeHelperBeginWrite( SSGazeHelperRecord * r ) { InterlockedIncrement( &r->seq ); }
inline void SSGazeHelperEndWrite( SSGazeHelperRecord * r ) { InterlockedIncrement( &r->seq ); }

// Reader side (app): copies a consistent snapshot; false if none could be taken.
inline bool SSGazeHelperRead( const SSGazeHelperRecord * r, SSGazeHelperRecord * out )
{
  for( int attempt = 0; attempt < 16; ++attempt )
  {
    LONG s1 = InterlockedCompareExchange( const_cast<volatile LONG *>( &r->seq ), 0, 0 );
    if( s1 & 1 ) { YieldProcessor(); continue; }
    CopyMemory( out, (const void *) r, sizeof( *out ) );
    MemoryBarrier();
    LONG s2 = InterlockedCompareExchange( const_cast<volatile LONG *>( &r->seq ), 0, 0 );
    if( s1 == s2 ) return out->magic == kSSGazeHelperMagic && out->version == kSSGazeHelperVersion;
  }
  return false;
}

#define SSGAZE_HELPER_EXE_NAME L"SimonSaysGaze64.exe"

#endif // SSGazeHelperProtocol_h
