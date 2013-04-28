#include "Common.h"
#include "PlayState.h"
#include "MenuState.h"

int main(int argc, char** argv)
{
	Logger::getPtr();
	TimeManager::getPtr();

	// create subsystems:
	OgreSubsystem ogre(1024,768,false);
	OISSubsystem ois;
	BulletSubsystem bull;
	GUISubsystem gui(1024,768);
	ALSubsystem aSys;

	// allocate engine and add subsystems
	Engine* eng = new Engine();
	eng->addSubsystem(&aSys);
	eng->addSubsystem(&ogre);
	eng->addSubsystem(&ois);
	eng->addSubsystem(&bull);
	eng->addSubsystem(&gui);

	// initialize the engine
	eng->init();

  //SoundPtr s = aSys.play2D("../media/audio/music.ogg");
  //s->setGain(0.f,0.3f,0.3f);
  //s->setLooping(false);

	// add game state
	eng->addState(new PlayState());
	//eng->addState(new MenuState());

	// start up the engine
	eng->start();

  //s->stop();
  //s.setNull();

	// delete the engine object
	delete eng;
	return 0;
}
