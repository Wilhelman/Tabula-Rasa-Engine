//#include "MemLeaks.h"
#include "trDefs.h"
#include "trApp.h"
#include "trLog.h"

// Old school memory leak detector and other random awesomeness
#ifdef _DEBUG
	//#define TEST_MEMORY_MANAGER
#include "mmgr/mmgr.h"
#endif

// We need to include this here because SDL overwrites main()
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

//#include "Brofiler/Brofiler.h"

//#pragma comment (lib, "Brofiler/ProfilerCore32.lib")

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

trApp* App = NULL;

int main(int argc, char* args[])
{
	//ReportMemoryLeaks();

	TR_LOG("Engine starting ... %d");

	MainState state = MainState::CREATE;
	int result = EXIT_FAILURE;

	while (state != EXIT)
	{
		switch (state)
		{

			// Allocate the engine --------------------------------------------
		case CREATE:
			TR_LOG("CREATION PHASE ===============================");

			App = new trApp(argc, args);

			if (App != NULL)
				state = AWAKE;
			else
				state = FAIL;

			break;

			// Awake all modules -----------------------------------------------
		case AWAKE:
			TR_LOG("AWAKE PHASE ===============================");
			if (App->Awake() == true)
				state = START;
			else
			{
				TR_LOG("ERROR: Awake failed");
				state = FAIL;
			}

			break;

			// Call all modules before first frame  ----------------------------
		case START:
			TR_LOG("START PHASE ===============================");
			if (App->Start() == true)
			{
				state = LOOP;
				TR_LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = FAIL;
				TR_LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
		case LOOP:
		{
			//BROFILER_FRAME("MainLoop");
			if (App->Update() == false)
				state = CLEAN;
		}
		break;

		// Cleanup allocated memory -----------------------------------------
		case CLEAN:
			TR_LOG("CLEANUP PHASE ===============================");
			if (App->CleanUp() == true)
			{
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
		case FAIL:
			TR_LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = EXIT;
			break;
		}
	}

	RELEASE(App);

	TR_LOG("... Bye! :)\n");
#ifdef _DEBUG
	int leaks = MAX(0, m_getMemoryStatistics().totalAllocUnitCount - 23);
	
	TR_LOG("With %d memory leaks!\n", (leaks > 0) ? leaks : 0);
#endif

	// Dump memory leaks
	return result;
}